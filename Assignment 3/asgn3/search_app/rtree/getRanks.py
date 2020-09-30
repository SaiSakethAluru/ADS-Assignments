import urllib.request as request
from bs4 import BeautifulSoup
from textblob import TextBlob
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize
from num2words import num2words
import pickle
import numpy as np
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_selection import SelectKBest, chi2
import pandas as pd
import math
from sklearn.metrics.pairwise import linear_kernel

import pickle

def remove_single(data):
    new_text = ""
    tokens = word_tokenize(str(data))
    for w in tokens:
        if len(w) > 1:
            new_text = new_text + " " + w
    return new_text

def convert_lower_case(data):
    return np.char.lower(data)

def remove_punctuation(data):
    symbols = "!\"#$%&()*+-./:;<=>?@[\]^_`{|}~\n"
    for i in range(len(symbols)):
        data = np.char.replace(data, symbols[i], ' ')
        data = np.char.replace(data, "  ", " ")
    data = np.char.replace(data, ',', '')
    return data

def remove_apostrophe(data):
    return np.char.replace(data, '\'', ' ')
    return np.char.replace(data, '"', '')

def stemming(data):
    stemmer= PorterStemmer()
    
    tokens = word_tokenize(str(data))
    new_text = ""
    for w in tokens:
        
        new_text = new_text + " " + stemmer.stem(w).lstrip("0")
    return new_text

def preprocess(data):
    data = convert_lower_case(data)
    data = remove_punctuation(data)
    data = remove_apostrophe(data)
    data = remove_single(data)
    data = stemming(data)
    data = remove_punctuation(data)
    data = remove_apostrophe(data)
    return data




with open('input.txt') as f:
    lines = f.readlines()

in_str = lines[1].strip()
k = int(lines[0].strip())

nouns = ' '.join(w for (w, pos) in TextBlob(in_str).pos_tags if (pos[0] == 'N' or pos[0] == 'C'))
in_str = preprocess(nouns)

vectorizer = pickle.load(open("../vectorizer.pickle", "rb"))
vectors = pickle.load(open("../vectors.pickle", "rb"))
features = vectorizer.get_feature_names()

new_doc = []
new_doc.append(str(in_str))
print("Searching",new_doc)
new_vector = vectorizer.transform(new_doc)

cosine_similarities = linear_kernel(new_vector, vectors).flatten()

# #Ranking docs based on cos_sims
# related_docs_indices = cosine_similarities.argsort()[:-10542:-1]

# #writing ranked order to output2.txt
# with open('output2.txt', 'w') as my_file:
#     for item in related_docs_indices:
#         my_file.write("%s\n" % item)

image_lst = []

with open('output.txt') as f:
    lines = f.readlines()

for line in lines:
    image_id = int(line.strip())
    cosine_sim = cosine_similarities[image_id]
    image_lst.append((image_id,cosine_sim))

image_lst = sorted(image_lst, key = lambda x: x[1])
image_lst = image_lst[::-1] 

final_lst = image_lst[0:k]
with open('final.txt', 'w') as my_file2:
    for item in final_lst:
        my_file2.write("%s\n" % str(item[0]))
        # print(item[1],item[0])
