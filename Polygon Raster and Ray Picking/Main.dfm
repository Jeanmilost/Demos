object MainForm: TMainForm
  Left = 0
  Top = 0
  Cursor = crCross
  Caption = 'Simple renderer and ray picking'
  ClientHeight = 500
  ClientWidth = 500
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object aeEvents: TApplicationEvents
    OnIdle = aeEventsIdle
    Left = 600
    Top = 8
  end
end
