const express = require('express');
const router = express.Router();
const { db } = require('../Firebase/firebase.js');

router.get('/pulse', async (req, res) => {
  try {
    const doc = await db.collection('PulseForge').doc("Patchnote").get();

    const result = {...doc.data()};
    res.json(result);
  } 
  catch (error) 
  {
    console.error(error);
    res.status(500).json({ error: 'Erreur serveur' });
  }
});

router.post("/ConnectUser", (req, res) =>
{
  console.log("BODY:", req.body);
  const mail = req.body["email"];
  const pass = req.body["password"];

  res.json("wWYQPNanuwcw0Xdo3xM6dDGcPz02");
})



module.exports = router;