# -*- coding: utf-8 -*-
"""
Created on Mon Mar  9 21:01:33 2020

@author: Robert Dinh
"""
import sys
import requests


    
def getItems(TID):
    URL = "http://apollo.humber.ca/~n01267335/CENG319/adminQuery.php"

    TID = str(TID[1:])

    PARAMS = {'TID':TID} 

    r = requests.get(url = URL, params = PARAMS) 

    data = r.json()
    

    #print (data["data"])

    itemArray = []
    for d in data["data"]:
        print (d["ItemName"] + " " + d["Quantity"])
        if (len(d["ItemName"]) > 14):
            itemArray.append(d["ItemName"][0:14] + " " + d["Quantity"])
        else:
            itemArray.append(d["ItemName"]+ " " + d["Quantity"])
            
        
    return itemArray
    