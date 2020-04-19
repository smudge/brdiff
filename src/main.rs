use brdiff::Image;

fn show_usage(program: String) {
    eprintln!(
        "usage:
  {} [--help] <base_image> <subtraction_image> <difference_image>\n",
        program
    );
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 3 {
        show_usage(args[0].clone());
    }

    let base_image = Image::new(args[1].clone());
    let subtraction_image = Image::new(args[2].clone());

    println!("Hello, world!");
}
