from PIL import Image, ImageDraw, ImageFont
import textwrap
import cv2
import numpy as np


def convert_to_image(transcript):
    # 表示させる文章
    text = transcript

    im = Image.open('edp2in9disp.png')
    draw = ImageDraw.Draw(im)  # drawオブジェクトを生成

    wrap_list = textwrap.wrap(text, 10)  # テキストをn文字で改行しリストwrap_listに代入
    font = ImageFont.truetype(
        '/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc', 28)  # フォントの指定
    line_counter = 0  # 行数のカウンター
    for line in wrap_list:  # wrap_listから1行づつ取り出しlineに代入
        y = line_counter*30  # y座標をline_counterに応じて下げる
        draw.multiline_text((5, y), line, fill=(
            0, 0, 0), font=font)  # 1行分の文字列を画像に描画
        line_counter = line_counter + 1  # 行数のカウンターに1

    im = im.rotate(90, expand=True)
    im.save('output.png', quality=95)
    im2 = cv2.imread("output.png", 0)
    ret2, im2 = cv2.threshold(im2, 0, 255, cv2.THRESH_OTSU)
    cv2.imwrite('output2.png', im2)

    im_gray = np.array(Image.open('output.png').convert('L'))
    np.savetxt('image.csv', im_gray, delimiter=',', fmt='%d')
    np.savetxt('image.dat', im_gray.flatten(), delimiter=',', fmt='%d')

    im_nichi = np.array(Image.open('output.png').convert('L'))
    im_nichi = im_nichi//255
    im_senddata = []
    for i in range(296):
        for j in range(16):
            nishin = 0
            for k in range(8):
                nishin += im_nichi[i][j*8+k]
                if k != 7:
                    nishin *= 2
            im_senddata.append(int(nishin))

    print(len(im_senddata))

    return im_senddata
