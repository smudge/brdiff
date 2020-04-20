use brdiff::Image;
use std::process::exit;

fn show_usage(program: String) {
    eprintln!(
        "usage:
  {} [--help] <base_image> <subtraction_image> <difference_image>\n",
        program
    );
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 4 {
        show_usage(args[0].clone());
        exit(1);
    }

    let base_image = match Image::from_path(args[1].clone()) {
        Ok(image) => image,
        Err(e) => panic!("Error opening {}:\n{}", args[1], e),
    };
    let subtraction_image = match Image::from_path(args[2].clone()) {
        Ok(image) => image,
        Err(e) => panic!("Error opening {}:\n{}", args[2], e),
    };

    let mut output_image = match base_image.subtract(&subtraction_image) {
        Ok(output_image) => output_image,
        Err(e) => panic!("Error subtracting images:\n{}", e),
    };

    output_image.set_path(args[3].clone());
    match output_image.save() {
        Ok(_) => (),
        Err(e) => eprintln!("Error writing {}:\n{}", args[3], e),
    }
}
