output_dir="bin"
output="ray_tracer"

mkdir -p $output_dir
gcc src/main.c -o $output_dir/$output \
    # && ./$output_dir/$output > image.ppm