
# Dymo CUPS printer driver

This repository is derived from the official Dymo CUPS printer driver for Linux by Vladimir Buzuev.


## Usage

With this driver, Dymo label printers are usable like any other printer via CUPS.

Example using lpr:

* print very long text on a tape:
```sh
 lpr -o landscape -o PageSize=24_mm__1___Label__Auto_ docs/test.txt
```

* set printing options specific to the LabelWriter driver
```sh
 lpr -o PageSize=30252_Address -o PrintQuality=Graphics -o PrintDensity=Light docs/test.txt
```

* set printing options specific to the LabelManager driver
```sh
 lpr -o PageSize=Address_Label -o CutOptions=ChainMarks -o LabelAlignment=Right -o TapeColor=1
```


## Supported devices

* DYMO LabelMANAGER 400
* DYMO LabelMANAGER 450
* DYMO LabelMANAGER PC
* DYMO LabelMANAGER PC II
* DYMO LabelManager PnP*
* DYMO LabelManager Wireless PnP
  * Note: `usb_modeswitch` required when connected via USB (`usb_storage` kernel module loaded)
* DYMO LabelPOINT 350
* DYMO LabelWriter 300
* DYMO LabelWriter 310
* DYMO LabelWriter 315
* DYMO LabelWriter 320
* DYMO LabelWriter 330
* DYMO LabelWriter 330 Tubo
* DYMO LabelWriter 400
* DYMO LabelWriter 400 Turbo
* DYMO LabelWriter 450
* DYMO LabelWriter 450 DUO Label
* DYMO LabelWriter 450 DUO Tape
* DYMO LabelWriter 450 Turbo
* DYMO LabelWriter 450 Twin Turbo
* DYMO LabelWriter 4XL
* DYMO LabelWriter DUO Label
* DYMO LabelWriter DUO Tape
* DYMO LabelWriter DUO Tape 128
* DYMO LabelWriter SE450
* DYMO LabelWriter Twin Turbo
