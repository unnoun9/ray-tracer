output_dir="bin"
output="ray_tracer"

mkdir -p $output_dir
g++ src/main.cpp -o $output_dir/$output
rm -f image.ppm && ./$output_dir/$output > image.ppm