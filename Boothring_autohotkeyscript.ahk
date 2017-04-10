
SendMode Input
SetWorkingDir %A_ScriptDir%

;******************************
RS232_Port = COM13
;******************************        
RS232_Settings = %RS232_Port%:baud=57600 parity=N data=8 stop=1 dtr=Off xon=off to=off odsr=off octs=off rts=off idsr=off
RS232_FileHandle:=RS232_Initialize(RS232_Settings)
start:
Loop

{
	IfWinExist, Social Booth
	{
		WinGetTitle, Title, Social Booth
		if Title contains BG-Capture
			{
			gosub senden
			gosub capture			
			}
		else if Title contains BG-ThankYou
			{
			gosub sendenprinter
			gosub capture
			}
			
}
	
}
Return

senden:
 command = animations`r
        RS232_Write(RS232_FileHandle, command)	
 command = count`r
        RS232_Write(RS232_FileHandle, command)
command = rainbo`r
        RS232_Write(RS232_FileHandle, command)
command = ..`r
        RS232_Write(RS232_FileHandle, command)
return

sendenprinter:
 command = animations`r
        RS232_Write(RS232_FileHandle, command)	
 command = printer`r
        RS232_Write(RS232_FileHandle, command)
command = ..`r
        RS232_Write(RS232_FileHandle, command)
sleep, 4000
goto start
return

f12::
;Create Color Dialog GUI
Gui, Add, Text, x0 y0 w300 h20 +left, Choose foreground Color
Gui, Add, Text, x0 y20 w40 h20 +Right, Red
Gui, Add, Text, x0 y40 w40 h20 +Right, Green
Gui, Add, Text, x0 y60 w40 h20 +Right, Blue
Gui, Add, Slider, x40 y20 w190 h20 AltSubmit +NoTicks +Range0-255 vsR gSliderSub, %Rval%
Gui, Add, Slider, x40 y40 w190 h20 AltSubmit +NoTicks +Range0-255 vsG gSliderSub, %Gval%
Gui, Add, Slider, x40 y60 w190 h20 AltSubmit +NoTicks +Range0-255 vsB gSliderSub, %Bval%

Gui, Add, Text, x230 y20 w45 h20 gEditSub veR +Limit3 +Number, %Rval%
Gui, Add, Text, x230 y40 w45 h20 gEditSub veG +Limit3 +Number, %Gval%
Gui, Add, Text, x230 y60 w45 h20 gEditSub veB +Limit3 +Number, %Bval%

Gui, Add, Progress, x285 y20 w60 h60 +Border Background%RGBval% vpC
Gui, Add, Text, x285 y20 w60 h60 +Border vtP cWhite +BackgroundTrans, Preview

Gui, Add, Text, x0 y100 w300 h20 +left, Choose Background Color
Gui, Add, Text, x0 y120 w40 h20 +Right, Red
Gui, Add, Text, x0 y140 w40 h20 +Right, Green
Gui, Add, Text, x0 y160 w40 h20 +Right, Blue
Gui, Add, Text, x0 y200 w300 h20 +left, set Countdown time
Gui, Add, Text, x0 y220 w40 h20 +Right, seconds

Gui, Add, Slider, x40 y120 w190 h20 AltSubmit +NoTicks +Range0-255 vbsR gSliderSub, %bRval%
Gui, Add, Slider, x40 y140 w190 h20 AltSubmit +NoTicks +Range0-255 vbsG gSliderSub, %bGval%
Gui, Add, Slider, x40 y160 w190 h20 AltSubmit +NoTicks +Range0-255 vbsB gSliderSub, %bBval%
Gui, Add, Slider, x40 y220 w190 h20 AltSubmit +NoTicks +Range1-20 vbsS gSliderSub, %bSval%


Gui, Add, Text, x230 y120 w45 h20 gEditSub vbeR +Limit3 +Number, %bRval%
Gui, Add, Text, x230 y140 w45 h20 gEditSub vbeG +Limit3 +Number, %bGval%
Gui, Add, Text, x230 y160 w45 h20 gEditSub vbeB +Limit3 +Number, %bBval%
Gui, Add, Text, x230 y220 w45 h20 gEditSub vbeS +Limit3 +Number, %bSval%

Gui, Add, Progress, x285 y120 w60 h60 +Border Background%bRGBval% vbpC

Gui, Add, Button, x120 y250 w110 h20 vbS gButtonSub, Set to Controller
Gui, Show, w351 h305, BoothRing Color Settings
return

EditSub:
    ;Get Values
    GuiControlGet,Rval,,eR
    GuiControlGet,Gval,,eG
    GuiControlGet,Bval,,eB
    GuiControlGet,bSval,,beS
    GuiControlGet,bRval,,beR
    GuiControlGet,bGval,,beG
    GuiControlGet,bBval,,beB
    GuiControlGet,brBval,,btbr
    
;Set preview
    gosub set
    ;Make Everything else aware

    GuiControl,,sR,%Rval%
    GuiControl,,sG,%Gval%
    GuiControl,,sB,%Bval%
    GuiControl,,bsS,%bSval%
    GuiControl,,bsR,%bRval%
    GuiControl,,bsG,%bGval%
    GuiControl,,bsB,%bBval%
    GuiControl,,bbr,%brBval%
return


SliderSub:
    ;Get Values
    GuiControlGet,Rval,,sR
    GuiControlGet,Gval,,sG
    GuiControlGet,Bval,,sB
    GuiControlGet,bSval,,bsS
    GuiControlGet,bRval,,bsR
    GuiControlGet,bGval,,bsG
    GuiControlGet,bBval,,bsB
    GuiControlGet,brBval,,bbr
    ;Set preview
    gosub set
    ;Make Everything else aware
    GuiControl,,eR,%Rval%
    GuiControl,,eG,%Gval%
    GuiControl,,eB,%Bval%
    GuiControl,,beS,%bSval%
    GuiControl,,beR,%bRval%
    GuiControl,,beG,%bGval%
    GuiControl,,beB,%bBval%
GuiControl,,bbr,%brBval%
return

set:
    ;Convert values to Hex
    RGBval:=RGB(Rval,Gval,Bval)
    bRGBval:=RGB(bRval,bGval,bBval)
    ;Display Tooltip
    ToolTip Red: %Rval%`nGreen: %Gval%`nBlue: %Bval%`nHex: %RGBval%
    ;Make tooltip disappear after 375 ms (3/8th of a second)
    SetTimer, RemoveToolTip, 375
    
;Apply colour to preview
    GuiControl,+Background%RGBval%,pC
    GuiControl,+Background%bRGBval%,bpC 
return

RemoveToolTip:
    SetTimer, RemoveToolTip, Off ;Turn timer off
    ToolTip ;Turn off tooltip
return

ButtonSub:
 command = settings`r
        RS232_Write(RS232_FileHandle, command)	

command = r_r_f`r
        RS232_Write(RS232_FileHandle, command)
command = %Rval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = r_g_f`r
        RS232_Write(RS232_FileHandle, command)
command = %Gval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = r_b_f`r
        RS232_Write(RS232_FileHandle, command)
command = %Bval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = r_r_b`r
        RS232_Write(RS232_FileHandle, command)
command = %bRval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = r_g_b`r
        RS232_Write(RS232_FileHandle, command)
command = %bGval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = r_b_b`r
        RS232_Write(RS232_FileHandle, command)
command = %bBval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)
command = seconds`r
        RS232_Write(RS232_FileHandle, command)
command = %bSval%
        RS232_Write(RS232_FileHandle, command)
command = `r
        RS232_Write(RS232_FileHandle, command)

command = ..`r
        RS232_Write(RS232_FileHandle, command)


;starten der Test Animation

command = anima`r
        RS232_Write(RS232_FileHandle, command)
command = on`r
        RS232_Write(RS232_FileHandle, command)
command = count`r
        RS232_Write(RS232_FileHandle, command)
command = ..`r
        RS232_Write(RS232_FileHandle, command)
    
;Remove '0x' prefix to hex color code, saving it directly to the clipboard
    ;StringReplace,Clipboard,RGBval,0x
    ;Display Last selected values... (these values can later be used), and Notify the user
    MsgBox,Please Restart the script!!!!!!!!!!
    ;Skip Directly GuiClose
GuiClose:
    ;Exit This Example script
    ExitApp
return

;Function to convert Decimal RGB to Hexadecimal RBG, Note: '0' (zero) padding is unnecessary
RGB(r, g, b) {
    ;Shift Numbers
    var:=(r << 16) + (g << 8) + b
    ;Save current A_FormatInteger
    OldFormat := A_FormatInteger
    ;Set Hex A_FormatInteger mode
    SetFormat, Integer, Hex
    ;Force decimal number to Hex number
    var += 0
    ;set original A_FormatInteger mode
    SetFormat, Integer, %OldFormat%
    return var
}




capture:
WinGetTitle, Title, Social Booth
		if Title contains BG-Review
		{
goto start			
}


else if Title contains BG-GIF
{
goto capture
}


else if Title contains BG-Filter 
{
goto capture
}



else if Title contains BG-Capture 
{
goto capture
}





RS232_Close(RS232_FileHandle)
ExitApp

;########################################################################
;#                      Initialize RS232 Port                           #
;########################################################################
RS232_Initialize(RS232_Settings)
{
  ;###### Extract/Format the RS232 COM Port Number ######
  ;7/23/08 Thanks krisky68 for finding/solving the bug in which RS232 COM Ports greater than 9 didn't work.
  StringSplit, RS232_Temp, RS232_Settings, `:
  RS232_Temp1_Len := StrLen(RS232_Temp1)  ;For COM Ports > 9 \\.\ needs to prepended to the COM Port name.
  If (RS232_Temp1_Len > 4)                   ;So the valid names are
    RS232_COM = \\.\%RS232_Temp1%             ; ... COM8  COM9   \\.\COM10  \\.\COM11  \\.\COM12 and so on...
  Else                                          ;
    RS232_COM = %RS232_Temp1%

  ;8/10/09 A BIG Thanks to trenton_xavier for figuring out how to make COM Ports greater than 9 work for USB-Serial Dongles.
  StringTrimLeft, RS232_Settings, RS232_Settings, RS232_Temp1_Len+1 ;Remove the COM number (+1 for the semicolon) for BuildCommDCB.
  ;MsgBox, RS232_COM=%RS232_COM% `nRS232_Settings=%RS232_Settings%

  ;###### Build RS232 COM DCB ######
  ;Creates the structure that contains the RS232 COM Port number, baud rate,...
  VarSetCapacity(DCB, 28)
  BCD_Result := DllCall("BuildCommDCB"
       ,"str" , RS232_Settings ;lpDef
       ,"UInt", &DCB)        ;lpDCB
  If (BCD_Result <> 1)
  {
    MsgBox, There is a problem with Serial Port communication. `nFailed Dll BuildCommDCB, BCD_Result=%BCD_Result% `nThe Script Will Now Exit.
    Exit
  }

  ;###### Create RS232 COM File ######
  ;Creates the RS232 COM Port File Handle
  RS232_FileHandle := DllCall("CreateFile"
       ,"Str" , RS232_COM     ;File Name         
       ,"UInt", 0xC0000000   ;Desired Access
       ,"UInt", 3            ;Safe Mode
       ,"UInt", 0            ;Security Attributes
       ,"UInt", 3            ;Creation Disposition
       ,"UInt", 0            ;Flags And Attributes
       ,"UInt", 0            ;Template File
       ,"Cdecl Int")

  If (RS232_FileHandle < 1)
  {
    MsgBox, There is a problem with Serial Port communication. `nFailed Dll CreateFile, RS232_FileHandle=%RS232_FileHandle% `nThe Script Will Now Exit.
    Exit
  }

  ;###### Set COM State ######
  ;Sets the RS232 COM Port number, baud rate,...
  SCS_Result := DllCall("SetCommState"
       ,"UInt", RS232_FileHandle ;File Handle
       ,"UInt", &DCB)          ;Pointer to DCB structure
  If (SCS_Result <> 1)
  {
    MsgBox, There is a problem with Serial Port communication. `nFailed Dll SetCommState, SCS_Result=%SCS_Result% `nThe Script Will Now Exit.
    RS232_Close(RS232_FileHandle)
    Exit
  }

  ;###### Create the SetCommTimeouts Structure ######
  ReadIntervalTimeout        = 0x00000001
  ReadTotalTimeoutMultiplier = 0x00000000
  ReadTotalTimeoutConstant   = 0x00000000
  WriteTotalTimeoutMultiplier= 0x00000000
  WriteTotalTimeoutConstant  = 0x00000000

  VarSetCapacity(Data, 20, 0) ; 5 * sizeof(DWORD)
  NumPut(ReadIntervalTimeout,         Data,  0, "UInt")
  NumPut(ReadTotalTimeoutMultiplier,  Data,  4, "UInt")
  NumPut(ReadTotalTimeoutConstant,    Data,  8, "UInt")
  NumPut(WriteTotalTimeoutMultiplier, Data, 12, "UInt")
  NumPut(WriteTotalTimeoutConstant,   Data, 16, "UInt")

  ;###### Set the RS232 COM Timeouts ######
  SCT_result := DllCall("SetCommTimeouts"
     ,"UInt", RS232_FileHandle ;File Handle
     ,"UInt", &Data)         ;Pointer to the data structure
  If (SCT_result <> 1)
  {
    MsgBox, There is a problem with Serial Port communication. `nFailed Dll SetCommState, SCT_result=%SCT_result% `nThe Script Will Now Exit.
    RS232_Close(RS232_FileHandle)
    Exit
  }
  
  Return %RS232_FileHandle%
}

;########################################################################
;#                         Close RS232 Port                             #
;########################################################################
RS232_Close(RS232_FileHandle)
{
  ;###### Close the COM File ######
  CH_result := DllCall("CloseHandle", "UInt", RS232_FileHandle)
  If (CH_result <> 1)
    MsgBox, Failed Dll CloseHandle CH_result=%CH_result%

  Return
}

;########################################################################
;#                         Write to RS232 Port                          #
;########################################################################
RS232_Write(RS232_FileHandle,Message)
{

  ;###### Write the data to the RS232 COM Port ######
  WF_Result := DllCall("WriteFile"
       ,"UInt" , RS232_FileHandle ;File Handle
       ,"UInt" , &Message          ;Pointer to string to send
       ,"UInt" , Strlen(Message)            ;Data Length
       ,"UInt*", Bytes_Sent     ;Returns pointer to num bytes sent
       ,"Int"  , "NULL")
  If (WF_Result <> 1 or Bytes_Sent <> Strlen(Message))
    MsgBox, Failed Dll WriteFile to RS232 COM, result=%WF_Result% `nData Length=%Data_Length% `nBytes_Sent=%Bytes_Sent%
}

;########################################################################
;#                         Read from RS232 port                         #
;########################################################################
RS232_Read(RS232_FileHandle,Num_Bytes,ByRef RS232_Bytes_Received)
{
  SetFormat, Integer, HEX

  ;Set the Data buffer size, prefill with 0x55 = ASCII character "U"
  ;VarSetCapacity won't assign anything less than 3 bytes. Meaning: If you
  ;  tell it you want 1 or 2 byte size variable it will give you 3.
  Data_Length  := VarSetCapacity(Data, Num_Bytes, 0x55)
  ;MsgBox, Data_Length=%Data_Length%

  ;###### Read the data from the RS232 COM Port ######
  ;MsgBox, RS232_FileHandle=%RS232_FileHandle% `nNum_Bytes=%Num_Bytes%
  Read_Result := DllCall("ReadFile"
       ,"UInt" , RS232_FileHandle   ; hFile
       ,"Str"  , Data             ; lpBuffer
       ,"Int"  , Num_Bytes        ; nNumberOfBytesToRead
       ,"UInt*", RS232_Bytes_Received   ; lpNumberOfBytesReceived
       ,"Int"  , 0)               ; lpOverlapped

  ;MsgBox, RS232_FileHandle=%RS232_FileHandle% `nRead_Result=%Read_Result% `nBR=%RS232_Bytes_Received% ,`nData=%Data%
  If (Read_Result <> 1)
  {
    MsgBox, There is a problem with Serial Port communication. `nFailed Dll ReadFile on RS232 COM, result=%Read_Result% - The Script Will Now Exit.
    RS232_Close(RS232_FileHandle)
    Exit
  }

  Return Data
}
