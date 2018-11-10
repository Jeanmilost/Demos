object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Plasma'
  ClientHeight = 240
  ClientWidth = 320
  Color = clBtnFace
  Constraints.MaxHeight = 600
  Constraints.MaxWidth = 600
  Constraints.MinHeight = 100
  Constraints.MinWidth = 100
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object tiRefresh: TTimer
    Interval = 1
    OnTimer = tiRefreshTimer
    Left = 8
    Top = 8
  end
end
