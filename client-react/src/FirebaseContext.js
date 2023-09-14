import React, { useContext, createContext, useState, useEffect } from "react";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, set, onValue } from "firebase/database";
import { getAuth } from "firebase/auth";
import firebaseConfig from "./firebaseConfig";

const FirebaseContext = createContext()

const userTemplate = {
    "areas": [
        {
            name: "arae1",
            manualOpen: false,
            isOpen: false,
            plans: null
        },
        {
            name: "arae2",
            manualOpen: false,
            isOpen: false,
            plans: null
        },
        {
            name: "arae3",
            manualOpen: false,
            isOpen: false,
            plans: null
        },
        {
            name: "arae4",
            manualOpen: false,
            isOpen: false,
            plans: null
        }
    ],
    setting: {
        updateDelaySec: 60,
        maxTaps: 2
    }
}

// const planTemplate = {
//     "startTime": "08:00",
//     "duration": 0,
//     "repeat": 0,
//     "lastTime": "23.4.2023"
// }

export function FirebaseProvider({ children }) {
    const [userId, setUserId] = useState(null)
    const firebaseApp = initializeApp(firebaseConfig);
    const firebaseDB = getDatabase(firebaseApp);
    const firebaseAuth = getAuth(firebaseApp);
    const [data, setData] = useState(userTemplate)
    const dbRef = ref(firebaseDB, '/users/' + userId)
    const dbPath = '/users/' + userId

    useEffect(() => {
        firebaseAuth.onAuthStateChanged((user) => {
            if (user) {
                setUserId(user.uid)
            }
        });
    }, [firebaseAuth]);


    useEffect(() => {
        onValue(dbRef, (snapshot) => {
            setData(() => ({
                ...snapshot.val()
            }))
        })
    }, [userId])

    const updateDb = (dbRef, value) => {
        set(dbRef, value);
    }

    // const addNewUser = () => {
    //     updateDb()
    // }

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