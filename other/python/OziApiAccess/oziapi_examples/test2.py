import ctypes
from ctypes import *

class Ozi(object):
    def __init__(self):
        self.ozi_library=ctypes.CDLL("./OziAPI.dll")
        
        self.ozi_find_ozi = self.ozi_library.oziFindOzi
        self.ozi_find_ozi.restype = ctypes.c_int
        
        self.ozi_get_exe_path = self.ozi_library.oziGetExePath
        self.ozi_get_exe_path.restype = ctypes.c_int
        self.path = ctypes.c_char_p("".encode("ascii"))
        self.dataLength = ctypes.c_int(0)
        self.ozi_get_exe_path.argtypes = [ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_int)]
        

        self.ozi_create_track_point = self.ozi_library.oziCreateTrackPoint
        self.ozi_create_track_point.restype = ctypes.c_int
        self.pTrackNum = ctypes.c_int(1)
        self.pCode = ctypes.c_int(1)
        self.pLat = 59.57
        self.pLon = 30.19
        self.pAlt = 0.0
        self.pTpDate = 0.0
        self.ozi_create_track_point.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
#       
        self.ozi_refresh_map = self.ozi_library.oziRefreshMap
        self.ozi_refresh_map.restype = ctypes.c_int
        self.ozi_refresh_map.argtypes = []
        
        
        self.ozi_load_track_file = self.ozi_library.oziLoadTrackFile
        self.ozi_load_track_file.restype = ctypes.c_int
        self.pTrackNum = 2
        self.pFileName = ctypes.c_char_p("".encode("ascii"))
        
        self.ozi_load_track_file.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char_p)]
        
        
    def search(self):
        return self.ozi_search(self.m).value
        
    def findozi(self):
        return self.ozi_find_ozi()
        
    def refreshmap(self):
        return self.ozi_refresh_map()
        
    def getexepath(self):
        
        result = self.ozi_get_exe_path(ctypes.byref(self.path), ctypes.byref(self.dataLength))
        
        print(result)
        print(self.dataLength)
        
        strOziPath = cast(self.path, c_char_p).value
        strOziPath = strOziPath.decode("ascii")
        print(strOziPath)
        
        return result
        
    def createtrackpoint(self):
        
        result = self.ozi_create_track_point(self.pTrackNum, self.pCode, ctypes.c_double(self.pLat), ctypes.c_double(self.pLon), ctypes.c_double(self.pAlt), ctypes.c_double(self.pTpDate))
        self.pCode = ctypes.c_int(0)
        number = 1
        while number < 100:
            number += 1
            print(number)
            self.pLat = self.pLat + 0.001
            self.pLon = self.pLon + 0.001
            print(self.pLat)
            result = self.ozi_create_track_point(self.pTrackNum, self.pCode, ctypes.c_double(self.pLat), ctypes.c_double(self.pLon), ctypes.c_double(self.pAlt), ctypes.c_double(self.pTpDate))
            
            
    def loadtrackfile(self):
        self.pTrackNum = 2
        self.pFileName = ctypes.c_char_p("c:\\Users\\chepil\\Downloads\\ozi.plt".encode("ascii"))
        return self.ozi_load_track_file(ctypes.c_int(self.pTrackNum), ctypes.byref(self.pFileName))
                
r = Ozi()

print(r.findozi())
print(r.getexepath())

#print(r.createtrackpoint())
#print(r.refreshmap())

print(r.loadtrackfile())
print(r.refreshmap())





#def findOzi():
#   oziFindOziProto = ctypes.WINFUNCTYPE (
#       ctypes.c_int      # Return type.
#   )
#   oziFindOziApi = oziFindOziProto (("oziFindOzi", oziDll))
#   result = oziFindOziApi()
#   #print(result)
#   return result