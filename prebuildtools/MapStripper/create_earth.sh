rm ./img/*
rm ../../res/raw/earth.txt
./mapstripper -in ../Maps/earth_map.png -out earth.txt -size 15 -no-duplicate -out-images-from 100 -out-images-prefix earth_generated_ -out-images-dir img -bind e3e3e3 21 22 23 -bind 3a3a3a 11 12 13 14 15 16 -bind 93cb8c 31 -bind 8cbdcb 33 -bind d1ad66 35 -bind 6666d1 37
cp earth.txt ../../res/raw
cp ./img/* ../../res/img