import argparse
import os.path
from collections import OrderedDict

import cv2
import numpy as np


def tile_images(imgs, nc):
    nr = len(imgs) // nc
    return cv2.vconcat([
        cv2.hconcat(imgs[nc * r: min(len(imgs), nc * (r + 1))] + [np.zeros_like(imgs[0]) for _ in
                                                                  range(max(0, nc * (r + 1) - len(imgs)))])
        for r in range(nr)
    ])


def tile_frames(frame_dict, cols):
    return tile_images(
        [cv2.putText(img, tag, (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), thickness=5) for tag, img in
         frame_dict.items()], cols)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-c', '--cols', type=int, default=1)
    parser.add_argument('dirs', nargs='+')
    args = parser.parse_args()

    frame_dicts = map(
        lambda filename:
        OrderedDict(
            (os.path.basename(d), cv2.imread(os.path.join(d, filename)))
            for d in args.dirs
        ), sorted(os.listdir(args.dirs[0]))
    )

    writer = None
    for frame_dict in frame_dicts:
        tiled_frame = tile_frames(frame_dict, args.cols)
        if writer is None:
            fourcc = cv2.VideoWriter_fourcc(*'DIVX')
            fps = 10
            h, w = tiled_frame.shape[:2]
            writer = cv2.VideoWriter('output.avi', fourcc, fps, (w, h))
        writer.write(tiled_frame)
    writer.release()
