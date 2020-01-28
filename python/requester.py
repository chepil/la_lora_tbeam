# importing the requests library 
import requests 
import time
from time import gmtime, strftime

# api-endpoint 
URL = "http://192.168.220.128/get?message=getHttpLog"

# defining a params dict for the parameters to be sent to the API 
PARAMS = {'message':'getHttpLog'} 
FILENAME = "requests.log"
while True:

    # sending get request and saving the response as response object 
    r = requests.get(url = URL, params = PARAMS) 

    # extracting data in json format 
    
    data = r.json() 

    results = data['results']
    i = 0
    l = len(results)
    if l > 0:
        while i < len(results):
            res = results[i]
            msg = res['msg']

            currentTime = strftime("%Y-%m-%d %H:%M:%S", gmtime())

            print(currentTime+","+msg)
            text_file = open(FILENAME, "wta")
            n = text_file.write(currentTime+","+msg+"\n")
            text_file.close()
            i += 1

    #latitude = data['results'][0]['geometry']['location']['lat'] 
    #print("Latitude:%s\nLongitude:%s\nFormatted Address:%s"
    #	%(latitude, longitude,formatted_address)) 

    time.sleep(5)


