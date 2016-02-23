#!/bin/bash
#sudo rfkill unlbock 2 // see index of hci0 with rfkill list 
sudo hciconfig hci0 up
sudo btmgmt le on

