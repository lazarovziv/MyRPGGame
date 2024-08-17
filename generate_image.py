import os
import sys

# os.chdir('graphics')


def layer_pngs(bg_png_file_name, fg_png_file_name, postfix_name):
    import cv2

    # navigating to the project directory for navigation to the graphics directory
    os.chdir('graphics/player/')
    bg_png_file_name = bg_png_file_name + '.png'
    fg_png_file_name = fg_png_file_name + '.png'
    
    background = cv2.imread(bg_png_file_name, cv2.IMREAD_UNCHANGED)
    foreground = cv2.imread(fg_png_file_name, cv2.IMREAD_UNCHANGED)

    # normalize alpha channels from 0-255 to 0-1
    alpha_background = background[:, :, 3] / 255.0
    alpha_foreground = foreground[:, :, 3] / 255.0

    # set adjusted colors
    for color in range(0, 3):
        background[:, :, color] = alpha_foreground * foreground[:, :, color] + \
                                  alpha_background * background[:, :, color] * (1 - alpha_foreground)

    # set adjusted alpha and denormalize back to 0-255
    background[:, :, 3] = (1 - (1 - alpha_foreground) * (1 - alpha_background)) * 255

    # set saved image file name
    bg_last_slash_idx = 0
    for i in range(len(bg_png_file_name)):
        if bg_png_file_name[i] == '/':
            bg_last_slash_idx = i

    bg_prefix = bg_png_file_name[bg_last_slash_idx + 1:-len(".png")]
    fg_last_slash_idx = 0
    for i in range(len(fg_png_file_name)):
        if fg_png_file_name[i] == '/':
            fg_last_slash_idx = i

    fg_prefix = fg_png_file_name[fg_last_slash_idx + 1:-len(".png")]
    print(bg_prefix)
    print(fg_prefix)

    final_file_name = f'{postfix_name}.png'
    # creating the file if it doesnt exists
    if not os.path.isfile(final_file_name):
        cv2.imwrite(final_file_name, background)


layer_pngs(sys.argv[1], sys.argv[2], sys.argv[3])


