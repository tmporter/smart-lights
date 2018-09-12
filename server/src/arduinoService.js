const fetch = require("node-fetch");

const setColor = async hex => {
   return await fetch(`http://10.0.0.191/mailbox/${hex}`);
};

module.exports = {
   setColor
};
