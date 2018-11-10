object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Sinus scroll'
  ClientHeight = 300
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -32
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 39
  object tiRefresh: TTimer
    Interval = 1
    OnTimer = tiRefreshTimer
    Left = 16
    Top = 16
  end
end
