#include <dirent.h>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>

#include <argparse.hpp>
#include <SiamMask/siammask.h>

bool dirExists(const std::string& path)
{
    struct stat info{};
    if (stat(path.c_str(), &info) != 0)
        return false;
    return info.st_mode & S_IFDIR;
}

std::vector<std::string> listDir(const std::string& path, const std::vector<std::string>& match_ending)
{
    static const auto ends_with = [](std::string const & value, std::string const & ending) -> bool
    {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    if(!dirExists(path)) {
        throw std::runtime_error(std::string("Directory not found: ") + path);
    }

    std::vector<std::string> files;
    DIR *dir = opendir(path.c_str());

    if(dir == nullptr)
        return files;

    struct dirent *pdirent;
    while ((pdirent = readdir(dir)) != nullptr) {
        std::string name(pdirent->d_name);
        for(const auto& ending : match_ending){
            if(ends_with(name, ending)) {
                files.push_back(path + "/" + name);
                break;
            }
        }
    }
    closedir(dir);

    return files;
}

void overlayMask(const cv::Mat& src, const cv::Mat& mask, cv::Mat& dst) {
    std::vector<cv::Mat> chans;
    cv::split(src, chans);
    cv::max(chans[2], mask, chans[2]);
    cv::merge(chans, dst);
}

void drawBox(
    cv::Mat& img, const cv::RotatedRect& box, const cv::Scalar& color,
    int thickness = 1, int lineType = cv::LINE_8, int shift = 0
) {
    cv::Point2f corners[4];
    box.points(corners);
    for(int i = 0; i < 4; ++i) {
        cv::line(img, corners[i], corners[(i + 1) % 4], color, thickness, lineType, shift);
    }
}

int main(int argc, const char* argv[]) try {
    argparse::ArgumentParser parser;
    parser.addArgument("-m", "--modeldir", 1, false);
    parser.addArgument("-c", "--config", 1, false);
    parser.addArgument("-q", "--queuesize", 1);
    parser.addArgument("-g", "--gamma", 1);
    parser.addArgument("-r", "--roi", 4);
    parser.addArgument("-s", "--savedir", 1);
    parser.addArgument("-d", "--drawbox", 0);
    parser.addFinalArgument("target");

    parser.parse(argc, argv);

    torch::Device device(torch::kCUDA);

    SiamMask siammask(parser.retrieve<std::string>("modeldir"), device);
    State state;
    state.load_config(parser.retrieve<std::string>("config"));
    state.max_queue_size = parser.gotArgument("queuesize") ?
                             parser.retrieve<size_t>("queuesize") : 0;
    float gamma = parser.gotArgument("gamma") ?
                            parser.retrieve<float>("gamma") : 0.0f;
    if (state.max_queue_size > 1) {
        for(size_t i = 0; i < state.max_queue_size; ++i)
            state.mask_weights.push_back(std::pow((i + 1) / (float)state.max_queue_size, gamma));
        std::cout << "Smoothing queue size: " << state.max_queue_size << std::endl
                  << "Smoothing weights: " << state.mask_weights << std::endl;
    }

    const std::string target_dir = parser.retrieve<std::string>("target");
    std::vector<std::string> image_files = listDir(target_dir, {"jpg", "png", "bmp"});
    std::sort(image_files.begin(), image_files.end());

    std::cout << image_files.size() << " images found in " << target_dir << std::endl;

    std::vector<cv::Mat> images;
    for(const auto& image_file : image_files) {
        images.push_back(cv::imread(image_file));
    }

    bool save_results = parser.gotArgument("savedir");
    std::vector<cv::Mat> results;
    const std::string save_dir = save_results ? parser.retrieve<std::string>("savedir") : "";
    if (save_results && mkdir(save_dir.c_str(), 0777) != 0) {
        std::cout << "Warning: failed to create save directory: " << save_dir << std::endl;
        save_results = false;
    }

    cv::namedWindow("SiamMask");
    int64 toc = 0;
    cv::Rect roi;
    if (parser.gotArgument("roi")) {
        std::vector<int> xywh = parser.retrieve<std::vector<int> >("roi");
        roi = cv::Rect(xywh[0], xywh[1], xywh[2], xywh[3]);
    } else {
        roi = cv::selectROI("SiamMask", images.front(), false);
    }
    std::cout << "Initial ROI: " << roi << std::endl;

    if(roi.empty())
        return EXIT_SUCCESS;

    for(unsigned long i = 0; i < images.size(); ++i) {
        int64 tic = cv::getTickCount();

        cv::Mat& src = images[i];

        if (i == 0) {
            std::cout << "Initializing..." << std::endl;
            siameseInit(state, siammask, src, roi, device);
            cv::rectangle(src, roi, cv::Scalar(0, 255, 0));
        } else {
            siameseTrack(state, siammask, src, device);
            overlayMask(src, state.mask, src);
            if (parser.gotArgument("drawbox"))
                drawBox(src, state.rotated_rect, cv::Scalar(0, 255, 0));
        }

        cv::imshow("SiamMask", src);
        toc += cv::getTickCount() - tic;
        cv::waitKey(1);
        if (save_results)
            results.push_back(src);
    }

    double total_time = toc / cv::getTickFrequency();
    double fps = image_files.size() / total_time;
    printf("SiamMask Time: %.1fs Speed: %.1ffps (with visulization!)\n", total_time, fps);

    if (save_results) {
        std::cout << "Saving result images at " << save_dir << " ..." << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            std::ostringstream sout;
            sout << save_dir << "/" << std::setfill('0') << std::setw(5) << i << ".png";
            cv::imwrite(sout.str(), results[i]);
        }
    }

    return EXIT_SUCCESS;
} catch (std::exception& e) {
    std::cout << "Exception thrown!\n" << e.what() << std::endl;
    return EXIT_FAILURE;
}

