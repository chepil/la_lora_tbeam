unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, clipbrd, ComCtrls, ExtCtrls, ExtDlgs;

type
  TForm1 = class(TForm)
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    Label1: TLabel;
    cMapSingleClickText: TEdit;
    clat1: TEdit;
    cLon1: TEdit;
    Label2: TLabel;
    Label3: TLabel;
    bCenterMap: TBitBtn;
    cMapSingleClickOn: TCheckBox;
    Label4: TLabel;
    cMapDoubleClickText: TEdit;
    cMapDblClickOn: TCheckBox;
    Label5: TLabel;
    cObjectClickText: TEdit;
    cObjectClickOn: TCheckBox;
    Label6: TLabel;
    TabSheet3: TTabSheet;
    bClearMFs: TBitBtn;
    bClearMCs: TBitBtn;
    cMFlat: TEdit;
    cMFlon: TEdit;
    bCreateMF: TBitBtn;
    cMFname: TEdit;
    bCreateMC: TBitBtn;
    cMClat: TEdit;
    cMClon: TEdit;
    cMCname: TEdit;
    OziExplorer: TTabSheet;
    bOziExepath: TBitBtn;
    cOziExePath: TEdit;
    Label7: TLabel;
    TabSheet6: TTabSheet;
    Label8: TLabel;
    cMapDatum: TEdit;
    bGetMapDatum: TBitBtn;
    Label9: TLabel;
    Label10: TLabel;
    cDClat: TEdit;
    cDClon: TEdit;
    bConvertDatum: TBitBtn;
    bRepositionMF: TBitBtn;
    bOziCloseProgram: TBitBtn;
    bFindOzi: TBitBtn;
    bConvert2Grid: TBitBtn;
    TabSheet4: TTabSheet;
    Label11: TLabel;
    Label13: TLabel;
    cRMCstring: TEdit;
    BitBtn1: TBitBtn;
    Timer1: TTimer;
    BitBtn2: TBitBtn;
    bFindMap: TBitBtn;
    TabSheet5: TTabSheet;
    cLatTrack: TEdit;
    cLonTrack: TEdit;
    bCreateTP: TBitBtn;
    cTrackNo: TEdit;
    cCode: TEdit;
    TabSheet7: TTabSheet;
    bClearWps: TBitBtn;
    cWPlat: TEdit;
    cWPlon: TEdit;
    bCreateWP: TBitBtn;
    cWPname: TEdit;
    cWpNumber: TEdit;
    bGetWpNumber: TBitBtn;
    bDeleteWpByName: TBitBtn;
    bDeleteWpByNumber: TBitBtn;
    bDeleteMC: TBitBtn;
    bDeleteMF: TBitBtn;
    procedure bCenterMapClick(Sender: TObject);
    procedure cMapSingleClickOnClick(Sender: TObject);
    procedure cMapDblClickOnClick(Sender: TObject);
    procedure cObjectClickOnClick(Sender: TObject);
    procedure bClearWpsClick(Sender: TObject);
    procedure bClearMFsClick(Sender: TObject);
    procedure bClearMCsClick(Sender: TObject);
    procedure bCreateMFClick(Sender: TObject);
    procedure bCreateMCClick(Sender: TObject);
    procedure bOziExepathClick(Sender: TObject);
    procedure bOziCloseProgramClick(Sender: TObject);
    procedure bFindOziClick(Sender: TObject);
    procedure bGetMapDatumClick(Sender: TObject);
    procedure bConvertDatumClick(Sender: TObject);
    procedure bCreateWPClick(Sender: TObject);
    procedure bRepositionMFClick(Sender: TObject);
    procedure bConvert2GridClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure bFindMapClick(Sender: TObject);
    procedure bCreateTPClick(Sender: TObject);
    procedure bGetWpNumberClick(Sender: TObject);
    procedure bDeleteWpByNameClick(Sender: TObject);
    procedure bDeleteWpByNumberClick(Sender: TObject);
    procedure bDeleteMCClick(Sender: TObject);
    procedure bDeleteMFClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form1: TForm1;


function oziFindOzi:integer; stdcall; external 'oziapi.dll';
function oziCloseProgram:integer; stdcall; external 'oziapi.dll';
function oziGetExePath(var p:pchar;var DataLength:integer): integer; stdcall; external 'oziapi.dll';
function oziCenterMapAtPosition(lat,lon:double): integer; stdcall; external 'oziapi.dll';
function oziFindMapAtPosition(lat,lon:double): integer; stdcall; external 'oziapi.dll';
function oziGetMapDatum(var p:pchar;var DataLength:integer): integer; stdcall; external 'oziapi.dll';
function oziConvertDatum(var DatumFrom:pchar; var DatumTo:pchar; var lat:double; var lon:double):integer;stdcall; external 'oziapi.dll';

function oziMapSingleClickON(CallbackProc:pointer):integer; stdcall; external 'oziapi.dll';
function oziMapSingleClickOFF:integer; stdcall; external 'oziapi.dll';
function oziMapDblClickON(CallbackProc:pointer):integer; stdcall; external 'oziapi.dll';
function oziMapDblClickOFF:integer; stdcall; external 'oziapi.dll';
function oziObjectClickON(CallbackProc:pointer):integer; stdcall; external 'oziapi.dll';
function oziObjectClickOFF:integer; stdcall; external 'oziapi.dll';

function oziClearWPs:integer; stdcall; external 'oziapi.dll';
function oziClearEVs:integer; stdcall; external 'oziapi.dll';
function oziClearRTEs:integer; stdcall; external 'oziapi.dll';
function oziClearAllTracks:integer; stdcall; external 'oziapi.dll';
function oziClearPTs:integer; stdcall; external 'oziapi.dll';
function oziClearMFs:integer; stdcall; external 'oziapi.dll';
function oziClearMCs:integer; stdcall; external 'oziapi.dll';

function oziSaveMapFlag(Flag:boolean):integer; stdcall; external 'oziapi.dll';

function oziCreateWP(var Name:pchar;Symbol:integer;lat,lon:double;Altitude:double;wpDate:double
                      ;MapDisplayFormat:integer;PointerDirection:integer;GarminDisplayFormat:integer
                      ;ForeColor,BackColor:integer;ProximityDistance:integer;var Description:pchar
                      ;FontSize:integer;FontStyle:integer;SymbolSize:integer):integer;stdcall; external 'oziapi.dll';

function oziCreateWPex(var Name:pchar;Symbol:integer;lat,lon:double;Altitude:double;wpDate:double
                      ;MapDisplayFormat:integer;PointerDirection:integer;GarminDisplayFormat:integer
                      ;ForeColor,BackColor:integer;ProximityDistance:integer;var Description:pchar
                      ;FontSize:integer;FontStyle:integer;SymbolSize:integer
                      ;ProximitySymbolPos:integer;ProximityTime:double;ProximityRoute:integer
                      ;var FileAttachmentName:pchar;var ProximityFileAttachmentName:pchar
                      ;var ProximitySymbolName:pchar ):integer;stdcall; external 'oziapi.dll';

function oziCreateMF(var Name:pchar;lat,lon:double;var Description:pchar;var PictureName:pchar
                      ;var SymbolName:pchar; CreateWp:integer;var WpName:pchar):integer;stdcall; external 'oziapi.dll';
function oziCreateMC(var Name:pchar;lat,lon:double; ForeColor, BackColor, Width, Height,
                      FontSize, FontStyle:integer):integer;stdcall; external 'oziapi.dll';

function oziRepositionMF(Number:integer;lat,lon:double):integer;stdcall; external 'oziapi.dll';

function oziConvertLL2Grid(GridNumber:integer;lat,lon:double;var Datum:pchar;
                 var Zone:pchar; var Easting,Northing:double;
                 lat0,Lon0,lat1,Lat2,K0,X0,Y0:double):integer;stdcall;external 'oziapi.dll';


function oziStartMMapi:integer;stdcall;external 'oziapi.dll';
function oziStopMM:integer;stdcall;external 'oziapi.dll';
function oziSendMMstring(var NmeaString:pchar):integer;stdcall;external 'oziapi.dll';

function oziRefreshMap:integer;stdcall;external 'oziapi.dll';
function oziCreateTrackPoint(TrackNum,Code:integer;lat,lon,altitude,tpDate:double):integer;stdcall;external 'oziapi.dll';

// waypoints //////////
function oziGetWpNumberFromName(var name:pansichar):integer;stdcall;external 'oziapi.dll';
function oziDeleteWpByName(var name:pansichar):integer;stdcall;external 'oziapi.dll';
function oziDeleteWpByNumber(wpNum:integer):integer;stdcall;external 'oziapi.dll';
///////////////////////

function oziDeleteMcByNumber(mcNum:integer):integer;stdcall;external 'oziapi.dll';
function oziDeleteMfByNumber(mfNum:integer):integer;stdcall;external 'oziapi.dll';

implementation

{$R *.DFM}


/////////////////////////////////////////////////////////
//helper functions
/////////////////////////////////////////////////////////
procedure Delay(ms: LongInt);
VAR
  TickCount: LongInt;
Begin
  TickCount:= GetTickCount;
  While GetTickCount - TickCount < ms do application.processmessages; {see multi-tasking}
End;

FUNCTION Ltrim(tempstr:String):String;
{!!!!!!!!!trims any spaces on the left of a string!!!!!!!!!}
VAR i,J:INTEGER;
BEGIN
J:=Length(tempstr);
if j>0
   then begin
        i:=1;
        REPEAT
          IF tempstr[1]=' ' THEN Delete(tempstr,1,1);
          i:=i+1;
        UNTIL (Length(tempstr)=0) or (tempstr[1]<>' ') Or (i>J);
        end;
Ltrim:=tempstr;
END;    {end of trimleft}


FUNCTION Rtrim(strww : String) : String;
VAR i : INTEGER;
BEGIN
i := Length(strww)+1;
REPEAT
  i := i-1;
UNTIL (i=0) or (strww[i] <> ' ');
IF i <> 0 THEN Rtrim := Copy(strww, 1, i) ELSE Rtrim := '';
END;


FUNCTION AllTrim(s:String):String;
BEGIN
AllTrim:=Rtrim(Ltrim(s));
END;


function f2strfixed(f:double;w,d:integer):string;
begin
result:=alltrim(floattostrf(f,fffixed,w,d));
end;

FUNCTION  Value(s:String):double;
VAR Code,i : INTEGER;
    t : double;
BEGIN
s:=AllTrim(s);
for i:=1 to length(s) do if s[i]=',' then s[i]:='.';
Val(s,t,Code);
Value:=t;
END;


function TabReadString(s:string;Column:integer;DefaultStr:string):string;
var i,j,cc:integer;
begin
result:=DefaultStr;
cc:=1;  //column count
if column>1
   then begin
        for i:=1 to column-1
           do begin
              j:=pos(#9,s);
              if j>0 then begin inc(cc);delete(s,1,j);end;
              end;
        end;
if cc<>column then exit;
j:=pos(#9,s);
if j=0 then j:=length(s)+1;
if j>1 then result:=alltrim(copy(s,1,j-1));
end;
/////////////////////////////////////////////////////////
// end of helper functions
/////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////
// these are the call back functions for the OziApi.dll
/////////////////////////////////////////////////////////
procedure MapSingleClickCallback(var oType:pchar; x,y:integer; Lat,Lon:double;var UTMZone:pchar; easting,northing:double);stdcall;
var s:string;
    i:integer;
begin
form1.cMapSingleClickText.text:=oType+'  '+inttostr(x)+' '+inttostr(y);
end;

procedure MapDblClickCallback(var oType:pchar; x,y:integer; Lat,Lon:double;var UTMZone:pchar; easting,northing:double);stdcall;
var s:string;
    i:integer;
begin
form1.cMapDoubleClickText.text:=oType+'  '+inttostr(x)+' '+inttostr(y);
end;

procedure ObjectClickCallback(var oType:pchar; Lat,Lon:double;var UTMZone:pchar; easting,northing:double;var sName:pchar);stdcall;
var s:string;
    i:integer;
begin
form1.cObjectClickText.text:=oType+'  '+f2strfixed(lat,20,6)+' '+f2strfixed(lon,20,6)+' *'+sName+'*';
end;

/////////////////////////////////////////////////////////
// end of callback functions
/////////////////////////////////////////////////////////



//turn single map click on/off
procedure TForm1.cMapSingleClickOnClick(Sender: TObject);
var return:integer;
begin
if cMapSingleClickOn.checked
   then begin
        // tell OziExplorer to send map clicks to the clipboard
        return:=oziMapSingleClickON(@MapSingleClickCallback);
        end
   else begin
        // tell OziExplorer to NOT send map clicks to the clipboard
        return:=oziMapSingleClickOFF;
        end;
end;


//turn double map click on/off
procedure TForm1.cMapDblClickOnClick(Sender: TObject);
var return:boolean;
begin
if cMapDblClickOn.checked
   then begin
        // tell OziExplorer to send map double clicks to the clipboard
        oziMapDblClickON(@MapDblClickCallback);
        end
   else begin
        // tell OziExplorer to NOT send map double clicks to the clipboard
        oziMapDblClickOFF;
        end;
end;


//turn object click on/off
procedure TForm1.cObjectClickOnClick(Sender: TObject);
var return:boolean;
begin
if cObjectClickOn.checked
   then begin
        // tell OziExplorer to send object clicks to the clipboard
        oziObjectClickON(@ObjectClickCallback);
        end
   else begin
        // tell OziExplorer to NOT send object clicks to the clipboard
        oziObjectClickOFF;
        end;
end;


//center map on specified position
procedure TForm1.bCenterMapClick(Sender: TObject);
var lat,lon:double;
begin
// reposition the map to the specified lat,lon
// lat,lon must be in decimal degrees
lat:=value(clat1.text);
lon:=value(clon1.text);
oziCenterMapAtPosition(lat,lon);
end;


//clear all loaded waypoints
procedure TForm1.bClearWpsClick(Sender: TObject);
var return:integer;
begin
// tell OziExplorer to clear all the waypoints
return:=oziClearWPs;
end;

//clear all loaded map features
procedure TForm1.bClearMFsClick(Sender: TObject);
var return:boolean;
begin
// tell OziExplorer to clear all the map features
oziClearMFs;
end;

//clear all loaded map comments
procedure TForm1.bClearMCsClick(Sender: TObject);
var return:boolean;
begin
// tell OziExplorer to clear all the map comments
oziClearMCs;
end;


//create a new waypoint
procedure TForm1.bCreateWPClick(Sender: TObject);
var lat,lon:double;
    Name,Description:pchar;
    FileAttachmentName:pchar;
    ProximityFileAttachmentName:pchar;
    ProximitySymbolName:pchar;
    return:integer;
begin
//positions must be in the WGS 84 datum
//if necessary there is a datum transformation command in the API

//this is the function to create a WP in the API dll.
//function oziCreateWP(var Name:pchar;Symbol:integer;lat,lon:double;Altitude:double;wpDate:double
//                      ;MapDisplayFormat:integer;PointerDirection:integer;GarminDisplayFormat:integer
//                      ;ForeColor,BackColor:integer;ProximityDistance:integer;var Description:pchar
//                      ;FontSize:integer;FontStyle:integer;SymbolSize:integer):integer;stdcall;

//function oziCreateWPex(var Name:pchar;Symbol:integer;lat,lon:double;Altitude:double;wpDate:double
//                      ;MapDisplayFormat:integer;PointerDirection:integer;GarminDisplayFormat:integer
//                      ;ForeColor,BackColor:integer;ProximityDistance:integer;var Description:pchar
//                      ;FontSize:integer;FontStyle:integer;SymbolSize:integer
//                      ;ProximitySymbolPos:integer;ProximityTime:double;ProximityRoute:integer
//                      ;var FileAttachmentName:pchar;var ProximityFileAttachmentName:pchar
//                      ;var ProximitySymbolName:pchar ):integer;stdcall;


//set up the variables
lat:=value(cWPlat.text);
lon:=value(cWPlon.text);
Name:=pchar(cWPname.text);
Description:='My Description';

FileAttachmentName:='File Attachment Name';
ProximityFileAttachmentName:='Proximity File Attachment Name';
ProximitySymbolName:='Proximity Symbol Name';

return:=oziCreateWPex(Name,-1,lat,lon,-777,-1,
                    -1,-1,-1,
                    -1,-1,0,
                    Description,
                    -1,-1,-1,
                    2,10.0,1,
                    FileAttachmentName,
                    ProximityFileAttachmentName,
                    ProximitySymbolName   );
end;



//create a new map feature
procedure TForm1.bCreateMFClick(Sender: TObject);
var lat,lon:double;
    Name,Description,PictureName,SymbolName,WpName:pchar;
    return:integer;
begin
//positions must be in the WGS 84 datum
//if necessary there is a datum transformation command in the API

//set up the variables

lat:=value(cMFlat.text);
lon:=value(cMFlon.text);
Name:=pchar(cMFname.text);
Description:='My Description';
PictureName:='';
SymbolName:='';
WpName:='MyName';
return:=oziCreateMF(Name,lat,lon,Description,PictureName,SymbolName,1,WpName);

//Set the Save Map flag to true
return:=oziSaveMapFlag(true);
end;


//create a new map comment
procedure TForm1.bCreateMCClick(Sender: TObject);
var lat,lon:double;
    Name:pchar;
    ForeColor,BackColor,Width,Height,FontSize,FontStyle:integer;
    return:integer;
begin

lat:=value(cMClat.text);
lon:=value(cMClon.text);
Name:=pchar(cMCname.text);
ForeColor:=clBlack;
BackColor:=clRed;
Width:=200;
Height:=25;
FontSize:=14;
FontStyle:=1;
oziCreateMC(Name,lat,lon,ForeColor, BackColor, Width, Height, FontSize, FontStyle);

//Set the Save Map flag to true
return:=oziSaveMapFlag(true);
end;


procedure TForm1.bOziExepathClick(Sender: TObject);
var i:integer;
    s:string;
    p:pchar;
    DataLength:integer;
begin
oziGetExePath(p,DataLength);
s:='';
if p<>nil then for i:=0 to DataLength-1 do s:=s+p[i];
form1.cOziExepath.text:=s;
end;

procedure TForm1.bOziCloseProgramClick(Sender: TObject);
begin
oziCloseProgram;
end;

procedure TForm1.bFindOziClick(Sender: TObject);
var i:integer;
begin
i:=oziFindOzi;
if i<>0
   then ShowMessage('OziExplorer is not running');
end;

procedure TForm1.bGetMapDatumClick(Sender: TObject);
var i:integer;
    s:string;
    p:pchar;
    DataLength:integer;
begin
oziGetMapDatum(p,DataLength);
s:='';
if p<>nil then for i:=0 to DataLength-1 do s:=s+p[i];
form1.cMapDatum.text:=s;
end;

procedure TForm1.bConvertDatumClick(Sender: TObject);
var lat,lon:double;
    DatumFrom,DatumTo:pchar;
begin
lat:=value(cDClat.text);
lon:=value(cDClon.text);
DatumFrom:='Afgooye';
DatumTo:='WGS 84';
oziConvertDatum(DatumFrom,DatumTo,lat,lon);
cDClat.text:=f2strfixed(lat,20,11);
cDClon.text:=f2strfixed(lon,20,11);
end;

procedure TForm1.bRepositionMFClick(Sender: TObject);
var lat,lon:double;
    Name,Description,PictureName,SymbolName,WpName:pchar;
    return:integer;
begin
//positions must be in the WGS 84 datum
//if necessary there is a datum transformation command in the API
//set up the variables
lat:=-26.495;
lon:=152.325;
return:=oziRepositionMF(1,lat,lon);
end;

procedure TForm1.bConvert2GridClick(Sender: TObject);
var lat,lon:double;
    return:integer;
    s:string;
    Datum,Zone:pchar;
    Easting,Northing:double;
begin
//function oziConvertLL2Grid(GridNumber:integer;lat,lon:double;var Datum:pchar;
//                 var Zone:pchar; var Easting,Northing:double;
//                 lat0,Lon0,lat1,Lat2,K0,X0,Y0:double):integer;stdcall;


//british National Grid
//lat:=52.45;
//lon:=-1.5;
//Datum:='Ord Srvy Grt Britn';
//return:=oziConvertLL2Grid(3,lat,lon,Datum,Zone,Easting,Northing,0,0,0,0,0,0,0);


//UTM
lat:=-26.495;
lon:=152.325;
Datum:='WGS 84';
return:=oziConvertLL2Grid(1,lat,lon,Datum,Zone,Easting,Northing,0,0,0,0,0,0,0);

s:=Zone+'  '+f2strfixed(Easting,20,0)+'  '+f2strfixed(Northing,20,0);
form1.cMapDatum.text:=s;

end;

procedure TForm1.BitBtn1Click(Sender: TObject);
begin
oziStartMMapi;
Timer1.Enabled:=true;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var s:string;
    p:pchar;
begin
s:=cRMCstring.Text;
p:=pchar(s);
oziSendMMstring(p);
end;

procedure TForm1.BitBtn2Click(Sender: TObject);
begin
oziStopMM;
Timer1.Enabled:=false;
end;

procedure TForm1.bFindMapClick(Sender: TObject);
var lat,lon:double;
begin
// reposition the map to the specified lat,lon
// lat,lon must be in decimal degrees
lat:=value(clat1.text);
lon:=value(clon1.text);
oziFindMapAtPosition(lat,lon);
end;

procedure TForm1.bCreateTPClick(Sender: TObject);
var TrackNo,Code:integer;
    tpDate:TDateTime;
    lat,lon:double;
    Altitude:double;
begin
TrackNo:=strtoint(cTrackNo.text);
Code:=strtoint(cCode.text);
lat:=value(clatTrack.text);
lon:=value(clonTrack.text);
Altitude:=-777; //no altitude
tpDate:=now;

oziCreateTrackPoint(TrackNo,Code,lat,lon,altitude,tpDate);
oziCreateTrackPoint(TrackNo,Code,lat,lon+0.05,altitude,tpDate);
oziCreateTrackPoint(TrackNo,1,lat+0.05,lon+0.03,altitude,tpDate);
oziCreateTrackPoint(TrackNo,0,lat+0.07,lon+0.04,altitude,tpDate);
oziCreateTrackPoint(TrackNo,0,lat+0.12,lon+0.03,altitude,tpDate);

//make sure the map is redrawn to show the track
oziRefreshMap;

end;

procedure TForm1.bGetWpNumberClick(Sender: TObject);
var lat,lon:double;
    s:string;
    Name,Description,PictureName,SymbolName,WpName:pchar;
    return:integer;
begin
cWpNumber.text:='';
application.processmessages;
delay(100);
Name:=pchar(cWpName.text);
return:=oziGetWpNumberFromName(Name);
cWpNumber.text:=inttostr(return);
end;

procedure TForm1.bDeleteWpByNameClick(Sender: TObject);
var return:integer;
    Name:pchar;
begin
Name:=pchar(cWpName.text);
return:=oziDeleteWpByName(Name);
end;

procedure TForm1.bDeleteWpByNumberClick(Sender: TObject);
var return:integer;
    j:integer;
begin
j:=strtoint(cWpNumber.text);
return:=oziDeleteWpByNumber(j);
end;

procedure TForm1.bDeleteMCClick(Sender: TObject);
var return:integer;
    j:integer;
begin
j:=1;  //delete mc 1
return:=oziDeleteMcByNumber(j);
end;

procedure TForm1.bDeleteMFClick(Sender: TObject);
var return:integer;
    j:integer;
begin
j:=1;  //delete mf 1
return:=oziDeleteMfByNumber(j);
end;

