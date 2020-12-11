print("test script")

function test()
  print("testing")
  wnd = FindWindow("Winamp v1.x")
  print("testing")
  SendMessage(wnd, "WM_COMMAND", 40045)
end
