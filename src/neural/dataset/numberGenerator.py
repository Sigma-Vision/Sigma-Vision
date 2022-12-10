from PIL import Image, ImageDraw, ImageFont, ImageFilter
#from zipfile import ZipFile
import random, os, shutil

NUMBER_OF_IMAGE = 500

#Check if the folder 1-9 exist
if os.path.exists("1"):
    shutil.rmtree("1", ignore_errors=False, onerror=None)

if os.path.exists("2"):
    shutil.rmtree("2", ignore_errors=False, onerror=None)

if os.path.exists("3"):
    shutil.rmtree("3", ignore_errors=False, onerror=None)

if os.path.exists("4"):
    shutil.rmtree("4", ignore_errors=False, onerror=None)

if os.path.exists("5"):
    shutil.rmtree("5", ignore_errors=False, onerror=None)

if os.path.exists("6"):
    shutil.rmtree("6", ignore_errors=False, onerror=None)

if os.path.exists("7"):
    shutil.rmtree("7", ignore_errors=False, onerror=None)

if os.path.exists("8"):
    shutil.rmtree("8", ignore_errors=False, onerror=None)

if os.path.exists("9"):
    shutil.rmtree("9", ignore_errors=False, onerror=None)

os.mkdir("1")
os.mkdir("2")
os.mkdir("3")
os.mkdir("4")
os.mkdir("5")
os.mkdir("6")
os.mkdir("7")
os.mkdir("8")
os.mkdir("9")

#Generate image with random numbers and font. Save them into the result folder
for n in range(1, 10):
    for i in range(NUMBER_OF_IMAGE):
        Fpath = "font/" + random.choice(os.listdir("font/"))

        img = Image.new('RGB', (15, 23), color = (255, 255, 255))
        fnt= ImageFont.truetype(Fpath, 17)

        d = ImageDraw.Draw(img)
        #d.text((random.randint(-2,6),random.randint(-4, -2)), str(n), font=fnt, fill=(0, 0, 0))
        d.text((2, -3), str(n), font=fnt, fill=(0, 0, 0))

        img = img.resize((20, 20))

        img.save(str(n) + "/" + str(i) + ".png")

#Create a zipfile archive and put all the result in it
#with ZipFile('numberTest.zip', 'w') as zipObj:
 #   arr = os.listdir("results\\")
  #  for file in arr:
   #     zipObj.write("results/" + file)

#Remove the results folder afterwards
#shutil.rmtree("results", ignore_errors=False, onerror=None)
