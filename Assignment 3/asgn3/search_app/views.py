from django.shortcuts import render
from .forms import QueryForm
from .models import QueryModel
# import cv2
from PIL import Image
import numpy as np
import os
import subprocess
import pickle

def get_app_path(appname = 'search_app'):
    path = os.path.join(os.getcwd(),appname)
    return path.replace(' ','\ ')

    
data = None
# Create your views here.
def homepage(request):
    context = {}
    if(request.method == "POST"):
        form = QueryForm(request.POST,request.FILES)
        if form.is_valid():
            k_value = form.cleaned_data.get('num_neigh_input')
            keywords = form.cleaned_data.get("keyword_input")
            image = form.cleaned_data.get('image_input')
            histogram = [0]*64
            img = Image.open(image.open())
            img.save(os.path.join(os.getcwd(),'search_app','static','search_app','query_images','query.jpg'))
            width,height = img.size
            pixels = list(img.getdata())
            total_pix = width * height
            for pix in pixels:
                bw_val = 0
                for val in pix:
                    bw_val += val
                bw_val /= (len(pix))
                histogram[int(bw_val/4)] += 1
            total_pix = float(float(total_pix) / 1000000)
            for i in range(0,64):
                histogram[i] = int(histogram[i]/total_pix)
            with open(os.path.join(os.getcwd(),'search_app','rtree','input.txt'),'w') as f:
                f.write(str(k_value)+'\n')
                f.write(keywords+'\n')
                for val in histogram:
                    f.write(str(int(val)) + ' ')
                f.write('\n')
            root_dir = os.getcwd()

            logfile = open('log.txt','w')
            os.chdir('search_app/rtree')
            cmd = ['g++','-std=c++11','getNN.cpp','-o','getNN']
            status = subprocess.call(cmd)
            print('subprocess finished, '+str(status))
            cmd = ['./getNN']
            status = subprocess.call(cmd,stdout=logfile,stderr=logfile)
            print('subprocess executed '+str(status))
            cmd=['python3','getRanks.py']
            status = subprocess.call(cmd,stdout=logfile,stderr=logfile)
            print('ranking generated, '+str(status))
            os.chdir(root_dir)
            logfile.close()
            images = []
            file_path = os.path.join(os.getcwd(),'search_app','rtree','final.txt')
            global data
            if(data is None):
                data_path = os.path.join(os.getcwd(),'search_app','Dataset.txt')
                with open(data_path,'r') as g:
                    data = g.read().split('\n')
            f = open(file_path,'r')
            indices = f.read().split('\n')
            for index in indices[:-1]:
                index = int(index)
                link_pair = data[index].split(' ')
                images.append({'doc_url':link_pair[0],'image_url':link_pair[1]})
            f.close()

            context['images'] = images
     
    else:
        form = QueryForm()
    context['form'] = form
    return render(request,"search_app/home.html",context)

