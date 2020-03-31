# -*- coding: utf-8 -*-
"""
Created on Mon Mar  9 21:01:33 2020

@author: Robert Dinh
"""
import sys
import requests


    
def getItems(TID):
    
    itemArray = []
    TID = str(TID[1:])
    if (TID[0] == 'R'):
        TID = str(TID[1:])
    
        URL = "http://apollo.humber.ca/~n01267335/CENG319/actionQuery.php"
        PARAMS = {'QReturnTID':TID}
    
        r = requests.get(url = URL, params = PARAMS) 
    
        data = r.json()
        
        itemArray.append("Return:")
        itemArray.append(str(data["Status"]))
        
        print(TID+" Return: " +data["Status"])
        
    else:
        URL = "http://apollo.humber.ca/~n01267335/CENG319/actionQuery.php"
        PARAMS = {'TID':TID}
    
        r = requests.get(url = URL, params = PARAMS) 
    
        data = r.json()
        
        itemArray.append("Queued:")
        itemArray.append(str(data["Status"]))
        
        print("Queued: " +data["Status"])
    
    
    
    
    URL = "http://apollo.humber.ca/~n01267335/CENG319/adminQuery.php"
    

    PARAMS = {'TID':TID} 

    r = requests.get(url = URL, params = PARAMS) 

    data = r.json()
    

    #print (data["data"])

    
    for d in data["data"]:
        print (d["ItemName"] + " " + d["Quantity"])
        if (len(d["ItemName"]) > 14):
            itemArray.append(d["ItemName"][0:14] + " " + d["Quantity"])
        else:
            itemArray.append(d["ItemName"]+ " " + d["Quantity"])
            
        
    return itemArray
    