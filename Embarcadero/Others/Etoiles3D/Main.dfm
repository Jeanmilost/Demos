object MainForm: TMainForm
  Left = 315
  Top = 110
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Etoiles 3D'
  ClientHeight = 400
  ClientWidth = 400
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object tmDisplay: TTimer
    Interval = 1
    OnTimer = tmDisplayTimer
    Left = 8
    Top = 8
  end
end
