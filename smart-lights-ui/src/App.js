import React, { Component } from "react";
import { SwatchesPicker } from "react-color";

class App extends Component {
   state = {
      color: "#ffffff"
   };

   handleColorChange = color => {
      const hex = color.hex;
      this.setState({ color: hex });
      fetch(`http://10.0.0.191/mailbox/${hex.replace("#", "0x")}`);
   };

   render() {
      return (
         <div className="App">
            <SwatchesPicker
               width={1000}
               color={this.state.color}
               onChangeComplete={this.handleColorChange}
            />
         </div>
      );
   }
}

export default App;
