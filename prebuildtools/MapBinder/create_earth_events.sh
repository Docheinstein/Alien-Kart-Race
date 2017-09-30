./mapbinder -in ../Maps/earth_events.png -out earth_events.txt -size 15 -bind c600ff 0 -bind 000000 0 -bind 3a3a3a 1 -bind ffee00 2 -bind b0b0b0 3
rm ../../res/raw/earth_events.txt
cp earth_events.txt ../../res/raw