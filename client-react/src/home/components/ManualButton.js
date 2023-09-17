import { useEffect, useState } from "react";
import { ref } from "firebase/database";
import { useFirebase } from "../../FirebaseContext";
import style from './ManualButton.module.scss'

const modeTypes = {
    opening: {
        className: style.waiting,
        disabled: true,
        text: "Opening...",
    },
    closing: {
        className: style.waiting,
        disabled: true,
        text: "Closing...",
    },
    isOpen: {
        className: '',
        disabled: false,
        text: "Turn Off",
    },
    isClose: {
        className: style.disabled,
        disabled: true,
        text: "Off",
    }
}

const ManualButton = ({ isOpen, close, manualOpen, dbPathArea }) => {
    const { updateDb, firebaseDB } = useFirebase()
    const [mode, setMode] = useState(modeTypes.isClose)

    const closeRef = ref(firebaseDB, dbPathArea + '/close')
    const manualOpenRef = ref(firebaseDB, dbPathArea + '/manualOpen')

    const onClickHandler = () => {
        if (manualOpen) {
            updateDb(manualOpenRef, false)
        }
        updateDb(closeRef, true)
    }


    useEffect(() => {
        if (close) {
            if (isOpen) {
                setMode(modeTypes.closing)
            } else {
                setMode(modeTypes.isClose)
            }
        } else {
            if (isOpen) {
                setMode(modeTypes.isOpen)
            } else {
                if (manualOpen) {
                    setMode(modeTypes.opening)
                } else {
                    setMode(modeTypes.isClose)
                }
            }
        }
        // console.log('isOpen', isOpen, 'close', close, 'manualOpen', manualOpen)
    }, [isOpen, close, manualOpen])

    return (
        <div className={style.container}>
            <button className={`${style.button} ${mode.className}`}
                onClick={onClickHandler} disabled={mode.disabled}>
                {mode.text}
            </button>
        </div>
    );
}

export default ManualButton;