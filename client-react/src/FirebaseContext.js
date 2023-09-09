import React, { useContext, createContext, useState, useEffect } from "react";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";
import { getAuth } from "firebase/auth";
import firebaseConfig from "./firebaseConfig";

const FirebaseContext = createContext()

const userTemplate = {
    userId: null,
    displayName: null,
    area1: {
        name: 'arae1',
        isOpen: false,
        plans: [
            {
                startTime: 0,
                duration: 0,
                repeat: 0
            },
            {
                startTime: 1,
                duration: 0,
                repeat: 0
            }
        ]
    },
    area2: {
        name: 'arae2',
        isOpen: false,
        plans: [
            {
                startTime: 0,
                duration: 0,
                repeat: 0
            },
            {
                startTime: 1,
                duration: 0,
                repeat: 0
            }
        ]
    },
    area3: {
        name: 'arae3',
        isOpen: false,
        plans: [
            {
                startTime: 0,
                duration: 0,
                repeat: 0
            },
            {
                startTime: 1,
                duration: 0,
                repeat: 0
            }
        ]
    },
    area4: {
        name: 'arae4',
        isOpen: false,
        plans: [
            {
                startTime: 0,
                duration: 0,
                repeat: 0
            },
            {
                startTime: 1,
                duration: 0,
                repeat: 0
            }
        ]
    },
    setting: { updateDelaySec: 5000 }
}

export function FirebaseProvider({ children }) {
    const [userId, setUserId] = useState(null)
    const firebaseApp = initializeApp(firebaseConfig);
    const firebaseDB = getDatabase(firebaseApp);
    const firebaseAuth = getAuth(firebaseApp);
    const [data, setData] = useState(userTemplate)
    const dbRef = ref(firebaseDB, '/UsersData/' + userId)

    useEffect(() => {
        firebaseAuth.onAuthStateChanged((user) => {
            if (user) {
                setUserId(user.uid)
            }
        });
    }, [firebaseAuth]);


    useEffect(() => {
        onValue(dbRef, (snapshot) => {
            setData(data => ({
                ...data,
                ...snapshot.val()
            }))
        })
    }, [userId])

    return (
        <FirebaseContext.Provider value={{
            // data,
            userId,
            setUserId,
            // firebaseDB,
            firebaseAuth
        }}>
            {children}
        </FirebaseContext.Provider>
    );
}

export function useFirebase() {
    return useContext(FirebaseContext)
};