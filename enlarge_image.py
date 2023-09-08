import cv2 as cv
import os
import sys
import numpy as np

# only enlarges from the outside, need to enlarge from the inside
def enlarge(image_path, final_image_name):
    os.chdir('graphics/player')
    image = cv.imread(image_path, cv.IMREAD_UNCHANGED)

    total_width = 2 * image.shape[1]
    total_height = 2 * image.shape[0]

    final_image = np.zeros((total_height, total_width, 4), dtype=np.uint8)
    row_index = 0
    col_index = 0

    print(image.shape)
    temp_image = None
    for x in range(0, image.shape[0]-64, 64):
        for y in range(0, image.shape[1]-64, 64):
            temp_image = image[x:x+64, y:y+64]
            temp_image = cv.copyMakeBorder(temp_image, 32,  32, 32, 32, cv.BORDER_REPLICATE)
            final_image[row_index:row_index+128, col_index:col_index+128] = temp_image
            col_index += 128
        row_index += 128
        col_index = 0

    cv.imwrite(final_image_name, final_image)

enlarge(sys.argv[1], sys.argv[2])
