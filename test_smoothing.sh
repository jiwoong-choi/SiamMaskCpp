mkdir -p results/tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 --savedir results/tennis/original tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 2 -g 0 --savedir results/tennis/q2g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 2 -g 0.5 --savedir results/tennis/q2g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 2 -g 1 --savedir results/tennis/q2g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 3 -g 0 --savedir results/tennis/q3g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 3 -g 0.5 --savedir results/tennis/q3g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 3 -g 1 --savedir results/tennis/q3g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 3 -g 2 --savedir results/tennis/q3g2 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 4 -g 0 --savedir results/tennis/q4g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 4 -g 0.5 --savedir results/tennis/q4g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 4 -g 1 --savedir results/tennis/q4g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS -d --roi 306 112 170 253 -q 4 -g 2 --savedir results/tennis/q4g2 tennis
python make_video.py -c 4 `ls -d results/tennis/*`
mv output.avi tennis.avi

mkdir -p results/mask_only
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 --savedir results/mask_only/original tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 2 -g 0 --savedir results/mask_only/q2g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 2 -g 0.5 --savedir results/mask_only/q2g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 2 -g 1 --savedir results/mask_only/q2g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 3 -g 0 --savedir results/mask_only/q3g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 3 -g 0.5 --savedir results/mask_only/q3g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 3 -g 1 --savedir results/mask_only/q3g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 3 -g 2 --savedir results/mask_only/q3g2 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 4 -g 0 --savedir results/mask_only/q4g0 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 4 -g 0.5 --savedir results/mask_only/q4g0.5 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 4 -g 1 --savedir results/mask_only/q4g1 tennis
./demo -c config_davis.json -m models/SiamMask_DAVIS --roi 306 112 170 253 -q 4 -g 2 --savedir results/mask_only/q4g2 tennis
python make_video.py -c 4 `ls -d results/mask_only/*`
mv output.avi mask_only.avi
