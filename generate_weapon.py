from PIL import Image

def overlay_images(base_image_path, overlay_image_path, x_base, y_base, output_path):
    # Open the base image
    base_image = Image.open(base_image_path)
    base_image = crop_image(base_image, x_base, x_base + 64*6, y_base, y_base + 64*4)
    base_image_width, base_image_height = base_image.size
    # Open the overlay image
    overlay_image = Image.open(overlay_image_path)

    # Calculate the width and height of the overlay image that fits within the base image
    overlay_width, overlay_height = overlay_image.size
    # new_overlay_width = 64*6
    # new_overlay_height = 64*4
    # overlay_image = overlay_image.resize((new_overlay_width, new_overlay_height))
    # overlay_width, overlay_height = overlay_image.size

    # max_width = min(overlay_width, base_image.width - x_base)
    # max_height = min(overlay_height, base_image.height - y_base)

    # Resize the overlay image to fit within the calculated dimensions
    overlay_image = overlay_image.resize((base_image_width, base_image_height))

    # Paste the overlay image onto the base image
    base_image.paste(overlay_image, (x_base, y_base), overlay_image)

    # Save the result
    base_image.save(output_path)

def crop_image(image, x_start, x_end, y_start, y_end):
    pixels = []
    for y in range(y_start, y_end):
        y_pixels = []
        for x in range(x_start, x_end):
            y_pixels.append(image.getpixel((x, y)))
        pixels.append(y_pixels)
    new_image = Image.new('RGB', (len(pixels[0]), len(pixels)))
    for y in range(len(pixels)):
        for x in range(len(pixels[0])):
            new_image.putpixel((x, y), tuple(pixels[y][x]))
    return new_image

if __name__ == "__main__":
    base_image_path = "graphics/dark_chest_male.png"
    overlay_image_path = "graphics/images/weapons/oversize/right_hand/male/longsword_male.png"
    x_base = 0//2  # Starting x-coordinate on the base image
    y_base = 11*64  # Starting y-coordinate on the base image
    output_path = "output_image.png"

    overlay_images(base_image_path, overlay_image_path, x_base, y_base, output_path)