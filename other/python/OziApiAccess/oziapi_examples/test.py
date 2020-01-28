import ctypes
import time

from ctypes import *

# Load DLL into memory.

oziDll = ctypes.WinDLL("c:\\Users\\chepil\\Downloads\\oziapi_examples\\OziAPI.dll")

# Set up prototype and parameters for the desired function call.
# HLLAPI

def setApiTimeout(timeout):
	oziAPIsetTimeoutProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int,      # Return type.
		ctypes.c_void_p,   # Parameters 1
	)
	oziAPIsetTimeoutParams = (1, "timeout", 5000),
	pTimeout = ctypes.c_int (timeout)
	
	oziAPIsetTimeoutApi = oziAPIsetTimeoutProto (("oziAPIsetTimeout", oziDll), oziAPIsetTimeoutParams)
	result = oziAPIsetTimeoutApi(ctypes.byref (pTimeout))
	#print(result)
	
def findOzi():
	oziFindOziProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int      # Return type.
	)
	oziFindOziApi = oziFindOziProto (("oziFindOzi", oziDll))
	result = oziFindOziApi()
	#print(result)
	return result
	
def oziClose():
	oziCloseApiProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int      # Return type.
	)
	oziCloseApi = oziCloseApiProto (("oziCloseApi", oziDll))
	result = oziCloseApi()
	#print(result)
	return result

def oziLoadTrackFile(trackNum, fileName):
	
	oziLoadTrackFileProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int,      # Return type.
		ctypes.POINTER(ctypes.c_int),   # Parameters 1
		ctypes.POINTER(ctypes.c_char_p)   # Parameters 2
	)
	oziLoadTrackFileParams = (1, "TrackNum", 1), (1, "FileName", "".encode("ascii")),
	
	pTrackNum = ctypes.c_int (trackNum)
	
	fileNameAscii = (fileName + "\0").encode("ascii")
	print(fileNameAscii)
	
	size = 256
	print("size")
	print(size)
	pFileName_ptr = ctypes.create_string_buffer(fileNameAscii, size)
	
	#pFileName_ptr = ctypes.c_char_p()
	pFileName_ptr = ctypes.c_char_p(fileNameAscii) #c_char_p
	print(pFileName_ptr)

	#pFileName = LPCSTR(fileNameAscii) #c_char_p
	
	#test = cast(pFileName, c_char_p).value
	#test = test.decode("ascii")
	#print("test: ",test)
	#print()
	#print()
	
	oziLoadTrackFileApi = oziLoadTrackFileProto (("oziLoadTrackFile", oziDll), oziLoadTrackFileParams)
	print()
	print()
	result = oziLoadTrackFileApi(ctypes.byref (pTrackNum), ctypes.byref (pFileName_ptr))
	
	print(result)
	
def getExePath():
	
	oziGetExePathProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int,      # Return type.
		ctypes.POINTER(ctypes.c_char_p),   # Parameters 1
		ctypes.POINTER(ctypes.c_int)   # Parameters 2
	)
	oziGetExePathParams = (1, "Path", "".encode("ascii")), (1, "DataLength", 0),
	
	pPath = ctypes.c_char_p("".encode("ascii"))
	pDataLength = ctypes.c_int(0)
	
	oziGetExePathApi = oziGetExePathProto (("oziGetExePath", oziDll), oziGetExePathParams)
	
	result = oziGetExePathApi(ctypes.byref (pPath), ctypes.byref (pDataLength))
	
	print(result)
	strOziPath = cast(pPath, c_char_p).value
	strOziPath = strOziPath.decode("ascii")
	print("strOziPath: ",strOziPath)
	
	
#######
def createTrackPoint(trackNum, code, lat, lon, altitude, tpDate):
	
	oziCreateTrackPointProto = ctypes.WINFUNCTYPE (
    	ctypes.c_int,      # Return type.
		ctypes.c_void_p,   	# Parameters 1
		ctypes.c_void_p,   	# Parameters 2
		ctypes.c_void_p,   	# Parameters 3
		ctypes.c_void_p,   	# Parameters 4
		ctypes.c_void_p,   	# Parameters 5
		ctypes.c_void_p   	# Parameters 6
	)
	oziCreateTrackPointParams = (1, 'TrackNum', ctypes.c_int(0)), (1, 'Code', ctypes.c_int(0)), (1, 'lat', ctypes.c_longdouble(0)), (1, 'lng', ctypes.c_longdouble(0)), (1, 'altitude', ctypes.c_longdouble(0)),(1, 'tpDate', ctypes.c_longdouble(0)),
	
	pTrackNum = ctypes.c_int(trackNum)
	pCode = ctypes.c_int(code)
	pLat = ctypes.c_longdouble(lat)
	pLon = ctypes.c_longdouble(lon)
	pAlt = ctypes.c_longdouble(altitude)
	pTpDate = ctypes.c_longdouble(tpDate)
	
	oziCreateTrackPointApi = oziCreateTrackPointProto (("oziCreateTrackPoint", oziDll), oziCreateTrackPointParams)
	
	result = oziCreateTrackPointApi(ctypes.byref(pTrackNum), ctypes.byref(pCode), ctypes.byref(pLat), ctypes.byref(pLon), ctypes.byref(pAlt), ctypes.byref(pTpDate))
	
	print("createTrackPoint result:")
	print(result)
	
########
	
	
def do_main_cycle():
	found=findOzi()
	#oziClose()
	while (found==0):
		found=findOzi()
		#oziClose()
		print()
		print()
		print("get exe path:")
		getExePath()
		#oziClose()
		print()
		print()
		
		
		#print("create TP:")
		#createTrackPoint(1, 0, 59.0, 30.0, 0.0, 43835.0)
		#oziClose()
		#print()
		#print()
		#found = -1
		#exit(0)
		
		print("load track file:")
		oziLoadTrackFile(1, "c:\\Users\\chepil\\Downloads\\test.plt")
		#print("sleep 5")
		#time.sleep(5)
		print()
		print()
		oziClose()
		print()
		print()
		found = -1
		exit(0)
	print("ozi not found")
	oziClose()
	

	
	
setApiTimeout(5000)
found=findOzi()
if found==0:
	print("ozi found")
	do_main_cycle()
else:
	print("ozi not found")
	oziClose()
	

#hllApiProto = ctypes.WINFUNCTYPE (
#    ctypes.c_int,      # Return type.
#    ctypes.c_void_p,   # Parameters 1 ...
#    ctypes.c_void_p,
#    ctypes.c_void_p,
#    ctypes.c_void_p)   # ... thru 4.
#hllApiParams = (1, "p1", 0), (1, "p2", 0), (1, "p3",0), (1, "p4",0),

# Actually map the call ("HLLAPI(...)") to a Python name.

#hllApi = hllApiProto (("oziFindOzi", hllDll), hllApiParams)
#hllApi = hllApiProto (("oziFindOzi", hllDll))


# This is how you can actually call the DLL function.
# Set up the variables and call the Python name with them.

#p1 = ctypes.c_int (0)
#p2 = ctypes.c_char_p (sessionVar)
#p3 = ctypes.c_int (1)
#p4 = ctypes.c_int (0)
#hllApi (ctypes.byref (p1), p2, ctypes.byref (p3), ctypes.byref (p4))

#If OziExplorer is running the function returns 0 otherwise it returns -1
#result = hllApi()
#print(result)
