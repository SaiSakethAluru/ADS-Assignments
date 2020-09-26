from django.shortcuts import render
from .forms import QueryForm
from .models import QueryModel
import cv2
import numpy as np
import os

# Create your views here.
def homepage(request):
    context = {}
    if(request.method == "POST"):
        form = QueryForm(request.POST,request.FILES)
        if form.is_valid():
            keywords = form.cleaned_data.get("keyword_input")
            image = form.cleaned_data.get('image_input')
            img = cv2.imdecode(np.fromstring(request.FILES['image_input'].read(),np.uint8),cv2.IMREAD_UNCHANGED)
            img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
            histogram = cv2.calcHist([img],[0],None,[64],[0,256])
            # Generate tf-idf vector
            # Write them to file
            # Call search function. 
            images = []
            file_path = os.path.join(os.getcwd(),'search_app','output.txt')
            f = open(file_path,'r')
            links = f.read().split('\n')
            for link in links:
                link_pair = link.split(' ')
                images.append({'doc_url':link_pair[0],'image_url':link_pair[1]})
            f.close()
            context['images'] = images

                    
            
    else:
        form = QueryForm()
    context['form'] = form
    return render(request,"search_app/home.html",context)

