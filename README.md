
# Realtime Acoustic Detection Examples

The Realtime Acoustic Detection daemon (RADd) provides a framework for applying acoustic detection and classification algorithms to a near-realtime stream of audio.  It has been designed to run on debian linux or derivatives, specifically targetting [Gumstix](https://www.gumstix.com/) and [Raspbian on a Raspberry Pi](http://www.raspbian.org/).  It also has been tested on a [Beagleboard](beagleboard.org) running Ubuntu.

## Documentation
Documentation covers the configuration of hardware, writing of a detector/classifier and examining output.  It can be found in the [github wiki](https://github.com/CornellLabofOrnithology/brp-realtime-acoustic-detectors/wiki).

## Examples
Each folder contains a different example detector/classifier algorithm and configuration for use with RADd.

## Origins
The RADd system (see [run_detect](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6107182)) was developed by the Bioacoustic Reasearch Program, within the Cornell Lab of Ornithology, by Eric Spaulding, [Matt Robbins](https://github.com/matt-robbins) and Sam Fladung many moons ago.  

It's original and continued purpose is to alert commercial ships to the the presence of North Atlantic Right Whales in Massachusetts Bay, so they can slow to avoid collisions.  It's still operating today - http://www.listenforwhales.org/ .
