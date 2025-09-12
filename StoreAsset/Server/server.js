const express = require('express');
const app = express();

const authentification = require("./authentification/authentification");
const pulseForge = require("./PulseForge/PulseForge");

const version = "v0.0.1";

// Middleware de parsing JSON
app.use(express.json());

// Route GET simple
app.get('/', (req, res) => {
  res.send(`Asset Store server for Pulse Forge version ${version}`);
});

app.use("/auth", authentification);
app.use("/PulseForge", pulseForge);

// Lancer le serveur
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`✅ Server listening on http://localhost:${PORT}`);
});
