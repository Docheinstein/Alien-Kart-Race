./maptilesetgenerator -in ../Maps/earth_map.png -out-raw earth.txt -out-tileset earth_tileset.png -in-images-dir in -grid-size 15 -tileset-size 240 -no-duplicate -bind 3a3a3a street1.png street2.png street3.png street4.png street5.png street6.png -bind e3e3e3 grass1.png grass2.png grass3.png -bind d1ad66 left.png -bind 6666d1 right.png -bind 8cbdcb above.png -bind 93cb8c below.png -bind c600ff earth_bound_block.png
rm ../../res/raw/earth.txt
cp earth.txt ../../res/raw
rm ../../res/img/earth_tileset.png
cp earth_tileset.png ../../res/img