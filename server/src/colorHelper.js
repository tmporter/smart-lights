const colorMappings = {
   red: "0xff0000",
   orange: "",
   yellow: "",
   green: "0x00ff00",
   blue: "0x0000ff",
   purple: "",
   white: "0xffffff",
   black: "0x000000"
};

const getHexFromName = name => {
   return colorMappings[name] || colorMappings.white;
};

module.exports = {
   getHexFromName,
   colorMappings
};
