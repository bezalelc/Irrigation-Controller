import { useState } from 'react';
import { ref } from 'firebase/database';
import { GiTap } from 'react-icons/gi'
import InputText from '../sharedComponents/InputText';
import SubmitBUtton from '../sharedComponents/SubmitButton';
import { useFirebase } from '../FirebaseContext';
import { useNavigate } from 'react-router-dom';
import style from './AddArea.module.scss'

const AddArea = () => {
    const [newArea, setNewArea] = useState({
        activePlan: -1,
        close: false,
        fertilizer: false,
        isOpen: false,
        manualOpen: false,
        name: "",
        openTime: false,
        plans: false
    })
    const { updateDb, firebaseDB, data, dbPath } = useFirebase()
    const navigate = useNavigate()
    const [tapId, setTapId] = useState(false)



    const submit = () => {
        if (!newArea.name || tapId === false || (data.areas && tapId in data.areas)) {
            return false
        }
        if (!data.areas) {
            data.areas = {}
        }

        updateDb(ref(firebaseDB, dbPath + '/areas/' + tapId), newArea)
        navigate('/home')
    }

    const tapIdHandler = (event) => {
        setTapId("id" + event.target.value)
    }

    return (
        <div className={style.container}>
            <div className={style.title}>Add new Area</div>
            <form onSubmit={event => event.preventDefault()} className={style.form}>
                <InputText Icon={GiTap} type="text" placeholder="New Area's name" value={newArea.name} setValue={name => setNewArea(prev => ({
                    ...prev,
                    name: name
                }))} />
                <div className={style.tapId}>
                    <div className={style.tapIdText}>Select tap number</div>
                    <input className={style.input} type='number' min={0} max={31} onChange={tapIdHandler} required></input>
                </div>
                <SubmitBUtton text="Add" onClick={submit} className={style.button} />
            </form>
        </div>
    );
}

export default AddArea;