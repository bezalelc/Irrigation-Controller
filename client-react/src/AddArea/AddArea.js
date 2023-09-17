import { useState } from 'react';
import { ref } from 'firebase/database';
import { GiTap } from 'react-icons/gi'
import InputText from '../sharedComponents/InputText';
import SubmitBUtton from '../sharedComponents/SubmitButton';
import { useFirebase } from '../FirebaseContext';
import style from './AddArea.module.scss'
import { useNavigate } from 'react-router-dom';

const AddArea = () => {
    const [newArea, setNewArea] = useState({
        activePlan: -1,
        close: false,
        fertilizer: false,
        isOpen: false,
        manualOpen: false,
        name: "",
        openTime: "00:00:00",
        "plans": null
    })
    const { updateDb, firebaseDB, data, dbPath } = useFirebase()
    const areaRef = ref(firebaseDB, dbPath + '/areas')
    const navigate = useNavigate()



    const submit = () => {
        if (!newArea.name) {
            return
        }
        if (!data.areas) {
            data.areas = []
        }
        updateDb(areaRef, [...data.areas, newArea])
        navigate('/home')
    }

    return (
        <div className={style.container}>
            <div className={style.title}>Add new Area</div>
            <form onSubmit={event => event.preventDefault()} className={style.form}>
                <InputText Icon={GiTap} type="text" placeholder="New Area's name" value={newArea.name} setValue={name => setNewArea(prev => ({
                    ...prev,
                    name: name
                }))} />
                <SubmitBUtton text="Add" onClick={submit} className={style.button} />
            </form>
        </div>
    );
}

export default AddArea;