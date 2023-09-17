import React, { useContext, createContext, useState, useEffect } from "react";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, set, onValue } from "firebase/database";
import { getAuth } from "firebase/auth";
import firebaseConfig from "./firebaseConfig";

const FirebaseContext = createContext()

const userTemplate = {
    areas: null,
    setting: {
        updateDelaySec: 60,
        maxTaps: 2
    }
}

export function FirebaseProvider({ children }) {
    const [userId, setUserId] = useState(null)
    const firebaseApp = initializeApp(firebaseConfig);
    const firebaseDB = getDatabase(firebaseApp);
    const firebaseAuth = getAuth(firebaseApp);
    const [data, setData] = useState(userTemplate)
    const dbPath = '/users/' + userId

    useEffect(() => {
        firebaseAuth.onAuthStateChanged((user) => {
            if (user) {
                setUserId(user.uid)
            }
        });
    }, [firebaseAuth]);


    useEffect(() => {
        onValue(ref(firebaseDB, '/users/' + userId), (snapshot) => {
            setData(() => ({
                ...snapshot.val()
            }))
        })
    }, [userId, firebaseDB])

    const updateDb = (dbRef, value) => {
        set(dbRef, value);
    }

    return (
        <FirebaseContext.Provider value={{
            dbPath,
            updateDb,
            data,
            userId,
            setUserId,
            firebaseDB,
            firebaseAuth
        }}>
            {children}
        </FirebaseContext.Provider>
    );
}

export function useFirebase() {
    return useContext(FirebaseContext)
};