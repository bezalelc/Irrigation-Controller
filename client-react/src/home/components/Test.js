import { ref } from 'firebase/database';
import { useFirebase } from '../../FirebaseContext';
import style from './Test.module.scss'

const Test = ({ areaData, dbPathArea }) => {
    const { updateDb, firebaseDB } = useFirebase()

    return (
        <div className={style.container}>
            <button onClick={() => updateDb(ref(firebaseDB, dbPathArea + '/isOpen'), !areaData.isOpen)}>{areaData.isOpen ? "Open" : "Close"}</button>
            {/* <button>{areaData.fertilize ? "not fertilize" : "fertilize"}</button> */}
        </div>
    );
}

export default Test;