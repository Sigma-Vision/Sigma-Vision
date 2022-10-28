from PIL import Image, ImageDraw, ImageFont
from zipfile import ZipFile
import random, os, shutil

NUMBER_OF_IMAGE = 150

#Check if the folder results exist
if os.path.exists("results"):
    shutil.rmtree("results", ignore_errors=False, onerror=None)

os.mkdir("results")

#Generate image with random numbers and font. Save them into the result folder
for i in range(NUMBER_OF_IMAGE):
    Fpath = "font/" + random.choice(os.listdir("font\\"))

    img = Image.new('RGB', (15,23), color = (255, 255, 255))
    fnt = ImageFont.truetype(Fpath, 20)

    d = ImageDraw.Draw(img)
    d.text((random.randint(-2,6),random.randint(-4, -2)), str(random.randint(0, 9)), font=fnt, fill=(0, 0, 0))

    a = random.randint(0,2)
    if (a == 0):
        img = img.resize((8, 8))
    elif (a == 1):
        img = img.resize((16, 16))
    else:
        img = img.resize((32, 32))
    img.save("results/" + str(i) + ".png")

#Create a zipfile archive and put all the result in it
with ZipFile('numberTest.zip', 'w') as zipObj:
    arr = os.listdir("results\\")
    for file in arr:
        zipObj.write("results/" + file)

#Remove the results folder afterwards
shutil.rmtree("results", ignore_errors=False, onerror=None)