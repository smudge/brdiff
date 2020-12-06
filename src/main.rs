use brdiff::Image;
use std::process::exit;

fn show_usage(program: String) {
    eprintln!(
        "usage:
  {} [--help] [--invert] <base_image> <subtraction_image> <difference_image>\n",
        program
    );
}

fn main() {
    let mut invert = false;
    let mut args: Vec<String> = std::env::args().collect();
    let program = args[0].clone();
    args.remove(0);

    if args.len() > 0 && args[0] == "--invert" {
        args.remove(0);
        invert = true;
    }

    if args.len() != 3 {
        show_usage(program);
        exit(1);
    }

    let base_image = match Image::from_path(args[0].clone()) {
        Ok(image) => image,
        Err(e) => panic!("Error opening {}:\n{}", args[0], e),
    };
    let subtraction_image = match Image::from_path(args[1].clone()) {
        Ok(image) => image,
        Err(e) => panic!("Error opening {}:\n{}", args[1], e),
    };

    let mut output_image = match base_image.subtract(&subtraction_image, invert) {
        Ok(output_image) => output_image,
        Err(e) => panic!("Error subtracting images:\n{}", e),
    };

    output_image.set_path(args[2].clone());
    match output_image.save() {
        Ok(_) => (),
        Err(e) => eprintln!("Error writing {}:\n{}", args[2], e),
    }
}
