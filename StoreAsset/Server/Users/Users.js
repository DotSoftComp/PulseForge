const express = require('express');
const router = express.Router();
const { db } = require('../Firebase/firebase.js');

router.get('/:id', async (req, res) => {
  try {
    const doc = await db.collection('Users').doc(req.params.id).get();
    console.log("Doc exists?", doc.exists);
    console.log("Doc data:", doc.data());

    if (!doc.exists) {
      return res.status(404).json({ error: "User not found" });
    }

    const result = { id: doc.id, ...doc.data() };
    res.json(result);
  } 
  catch (error) {
    console.error(error);
    res.status(500).json({ error: "Erreur serveur" });
  }
});

module.exports = router;