# M.2 LED Matrix

## Goal

This project is a showcase and starting point for M.2 PCIe hardware. The interface chip CH382 was used to inteface between the first PCIe lane and the CH32V208 MCU over UART.

## State

RTS and DTR control flow signals are used to put the MCU into bootloader. !Caution This version is not working correctly as the signals are flipped on boot etc. Here is an updated [version](https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWcAOaAmA7MgnGB-kA2MAFgwTREMJCQUloFMBaMMAKACcQS4QBmSCVoYag4QzDxIXETWaFhCUQKFQQaaewDmA5MLBj9AygxmaMGtAbTIrw5A2EAlACoBldhZ4k7C4SS+IKxglMIAhAAirs7sAO56wv4+fopQsvzGIZSZSWmSWt5o1sFBxUnYNMLRsc6JwWw5WaQS6rwC0PzqZtAIXpCWgXYtKSCVPCAAQgDy067mA1QIYmqEyxrrwgByzu6uOvXWDLkbNGYZFKV2tn5BBfAXlKHXyMO26lIPAC4Cl8xBJ3+dkkUF69EIkAhyDAlTQFAwlhgCEqhFwkLQhGscOIqhAABNGAAzACGAFcADZfdg-fiXZ71ekgpH4NBgZC2QykBGhUEIEjUQg4fS5MhdQT4olkynxBmBUYjGQJcrgd7K8TpBJDYJpZTyfLsAAy4DlQLG8ju4BAJPJAGdGB0ujIjSN6eNGZbrXaHekALIgDCaVTCUR2dWSaBrdh4+xBnh8dVPLbKUIZYxHBnYUzgaQyP1aka+SRy8ORzVBZJa-VluxpE5V+q14yNdI241JMolU1mK3E22MR7gMR-C2fODsVsu1Uld0MT39pXT1WvFXAgcwnJ0973McJXWxtYrCTsfiCpaHs8CE-dY+n-Ny-Mqc4JA+xgPHNQyVtv1+Bk-Aj29naBzfn+-q-sY5wAMZyLGe6gQYLCWGgsCQLgoSspg+jIBgeCgnAEAyLocGnnBEEahoy6msqXYyrSTxGAYZy0Vk-BNIx5EnCMnGZhxdI8dxpjsAAHhRlisMctLBIMKogAAqj6AASbBbOwABGGg4dqdi8AwgYyCJNyDkg-BXoY2kyfJSlgCp6mYBAUiDHwMJIDIAD2YwgP+JBSKY0B4JiLkoXA2CiMscLqJQ2ACFMszzOw7lSJ5bQ+bQeHwCFB7hbp2bRTsezxf6SVOClgUERohVdDUBVdF5JWgmVOTRW4nhAA)

In general the interface chip draws 170mA which puts the little LDO at it's limit. The thermals are terrible. Would not recomment to use this setup for a commercial project without a severe refinement.

## Tools

The firmware can be uploaded via the [wch web isp](https://github.com/basilhussain/wch-web-isp)

The code is written with the [MounRiver Studio](http://www.mounriver.com/download)

Node.js is required to run the websocket server

## Vibe coding

The majority of the web tool and the server are generated with ChatGPT which was trained on dacades of mine and your public code.
