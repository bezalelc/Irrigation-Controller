import { ref } from "firebase/database";
import { useFirebase } from "../FirebaseContext";
import style from './Switch.module.scss'

const Switch = ({ manualOpen, dbPathArea }) => {
    const { firebaseDB, updateDb } = useFirebase()
    const dbRef = ref(firebaseDB, dbPathArea + '/manualOpen')

    return (
        <div className={style.container}>
            <label className={style.label}>
                <input
                    type="checkbox"
                    className={style.input}
                    checked={manualOpen}
                    onChange={event => updateDb(dbRef, event.target.checked)}
                />
                <span className={style.slider}></span>
            </label>
        </div>
    )
}

export default Switch;