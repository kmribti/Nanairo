/*!
  \file NRenderWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import "Items"
import "definitions.js" as Definitions

Window {
  id: renderWindow

  property bool isPreviewMode: false

  NLabel {
    id: infoLabel
    x: 0
    y: 0
    font.pixelSize: 10
    text: "000.00 fps,  00000000 cycles,  00 h 00 m 00.000 s"
  }

  NPane {
    id: renderFrame
    y: infoLabel.y + infoLabel.height
    width: renderWindow.width
    height: renderWindow.height - infoLabel.height

    Image {
      id: renderImage

      property int imageNumber

      anchors.fill: parent
      cache: false
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter
      smooth: false

      NPreviewEventArea {
        id: eventArea
        enabled: renderWindow.isPreviewMode
        anchors.fill: parent
      }

      function updateRenderImage() {
        imageNumber = imageNumber + 1;
        source = "image://renderedImage/preview?" + imageNumber;
      }
    }
  }

  Connections {
    target: nanairoManager
    onNotifyOfRenderingInfo: {
      infoLabel.text = info;
      renderImage.updateRenderImage();
    }
  }

  onClosing: nanairoManager.stopRendering()

  function initForRendering(imageResolution, isPreviewing) {
    width = imageResolution[0];
    height = imageResolution[1] + infoLabel.height;
    renderWindow.isPreviewMode = isPreviewing;
    renderImage.imageNumber = 0;
  }
}
