const express = require("express");
const morgan = require("morgan");
const { setColor } = require("./arduinoService");
const { getHexFromName, colorMappings } = require("./colorHelper");

const PORT = 3001;
const app = express();

app.use(morgan("tiny"));

app.get("/ping", (req, res) => {
   res.status(200).send("pong");
});

app.get("/color/name/:name", async (req, res) => {
   try {
      await setColor(getHexFromName(req.params.name));
      res.status(200).send({ message: "Successfully updated the color.", color: req.params.name });
   } catch (err) {
      console.error(err);
      res.status(500).send({ message: "An error occurred when trying to update the color.", error: err });
   }
});

app.get("/colors", (req, res) => {
   res.status(200).send(colorMappings);
});

app.get("/color/hex/:hex", async (req, res) => {
   try {
      await setColor(req.params.hex);
      res.status(200).send({ message: "Successfully updated the color.", color: req.params.hex });
   } catch (err) {
      console.error(err);
      res.status(500).send({ message: "An error occurred when trying to update the color.", error: err });
   }
});

app.listen(PORT, () => {
   console.log(`Server listening on port ${PORT}!`);
});
