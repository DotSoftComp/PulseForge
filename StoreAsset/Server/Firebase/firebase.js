const admin = require('firebase-admin');
const serviceAccount = require('./pulse-software-firebase-adminsdk-fbsvc-ba356465af.json');

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount)
});

const db = admin.firestore(); // pour Firestore
const auth = admin.auth();    // pour gérer l’auth

module.exports = { admin, db, auth };
