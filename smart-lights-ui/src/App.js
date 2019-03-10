import React, { Component } from "react";
import { SwatchesPicker, SketchPicker } from "react-color";

const mailboxUrl = "http://10.0.0.191/mailbox/";

class App extends Component {
  state = {
    color: "#ffffff",
    brightness: 50
  };

  handleBrightnessChange = brightness => {
    this.setState({ brightness });
    this.sendMailboxMessage("b" + brightness);
  };

  handleColorChange = color => {
    const hex = color.hex;
    this.setState({ color: hex });
    this.sendMailboxMessage(hex.replace("#", "0x"));
  };

  sendMailboxMessage = message => {
    fetch(mailboxUrl + message);
  };

  render() {
    return (
      <div className="App">
        <h3>Color</h3>
        <SketchPicker color={this.state.color} onChangeComplete={this.handleColorChange} disableAlpha />
        <SwatchesPicker width={1000} color={this.state.color} onChangeComplete={this.handleColorChange} />
        <h3>Brightness</h3>
        <div className="brightness-buttons">
          <BrightnessButton brightness={0} onClick={this.handleBrightnessChange} />
          <BrightnessButton brightness={25} onClick={this.handleBrightnessChange} />
          <BrightnessButton brightness={50} onClick={this.handleBrightnessChange} />
          <BrightnessButton brightness={75} onClick={this.handleBrightnessChange} />
          <BrightnessButton brightness={100} onClick={this.handleBrightnessChange} />
        </div>
      </div>
    );
  }
}

const BrightnessButton = ({ brightness, onClick }) => {
  return <button onClick={() => onClick(brightness)}>{brightness === 0 ? "Off" : brightness + "%"}</button>;
};

export default App;
