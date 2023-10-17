import { useState } from "react";
import { ref } from "firebase/database";
import { BsPencilFill } from 'react-icons/bs';
import { useFirebase } from "../FirebaseContext";
import EditPlan from "./EditPlan";
import Plan from "./Plan";
import Timer from "./Timer";
import CircularProgressBar from "./components/CircularProgressBar";
import Switch from "./Switch";
import Test from "./components/Test";
import ManualButton from "./components/ManualButton";
import style from './Area.module.scss'
import SubmitButton from "../sharedComponents/SubmitButton";

const Area = ({ id, areaData, dbPathAreas }) => {
    const { updateDb, firebaseDB, data } = useFirebase()
    const dbPathArea = dbPathAreas + `/${id}`
    const plansRef = ref(firebaseDB, dbPathArea + '/plans')
    const nameRef = ref(firebaseDB, dbPathArea + '/name')
    const manualOpenRef = ref(firebaseDB, dbPathArea + '/manualOpen')
    const fertilizerRef = ref(firebaseDB, dbPathArea + '/fertilizer')
    const [name, setName] = useState(areaData.name)
    const [addPlanPopUp, setAddPlanPopup] = useState(false)

    // Function to check if a new plan overlaps with existing plans
    function isPlanOverlapping(newPlan, planId) {
        const newStartTime = new Date(`2000-01-01T${newPlan.startTime}`);
        const newEndTime = new Date(newStartTime.getTime() + newPlan.duration * 60000);

        for (const key of Object.keys(areaData.plans)) {
            if (planId && key === planId) {
                continue
            }
            const existingPlan = areaData.plans[key]
            const existingStartTime = new Date(`2000-01-01T${existingPlan.startTime}`);
            const existingEndTime = new Date(existingStartTime.getTime() + existingPlan.duration * 60000);

            // Check if there is an overlap
            if (
                (newStartTime >= existingStartTime && newStartTime < existingEndTime) || // New plan starts within existing plan
                (newEndTime > existingStartTime && newEndTime <= existingEndTime) || // New plan ends within existing plan
                (newStartTime <= existingStartTime && newEndTime >= existingEndTime) // New plan completely overlaps existing plan
            ) {
                return true; // Overlap detected
            }
        }

        return false; // No overlap detected
    }

    const updatePlansSorted = (operation, plan, planId) => {
        if (!areaData.plans) {
            areaData.plans = plan ? { id0: plan } : false
        } else {
            if (operation === 'add' || operation === 'update') {
                if (isPlanOverlapping(plan, planId)) {
                    return false;
                }
            }

            if (operation === 'add') {
                let freeId = 0;
                for (; freeId < 8; freeId++) {
                    if (!areaData.plans["id" + freeId]) {
                        break
                    }
                }
                if (freeId === 8) {
                    return false
                }
                areaData.plans["id" + freeId] = plan
            } else if (operation === 'remove') {
                delete areaData.plans[planId]
            }
            else if (operation === 'update') {
                areaData.plans[planId] = plan
            }
        }
        updateDb(plansRef, areaData.plans)
    }

    const deleteArea = () => {
        delete data.areas[id]
        updateDb(ref(firebaseDB, dbPathAreas), data.areas)
    }

    return (
        <div className={style.container}>
            <div className={style.containerNav}>
                <div className={style.setting}>
                    <div className={style.firstRowContainer}>
                        <div className={style.setNameContainer} info="Edit Name">
                            <input type="text" value={name} className={style.input}
                                onChange={event => { setName(event.target.value) }}
                                onBlur={() => { updateDb(nameRef, name) }} />
                            <BsPencilFill className={style.icon} />
                        </div>
                        <SubmitButton text="Add plan" onClick={() => setAddPlanPopup(prev => !prev)} className={style.addPlan} />
                    </div>
                    <div className={style.tapId}>Tap number {id.substring(2)} </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Manual open</label>
                        <Switch value={areaData.manualOpen} dbRef={manualOpenRef} />
                    </div>
                    <div className={style.switchContainer}>
                        <label className={style.switchLabel}>Set as Fertilizer</label>
                        <Switch value={areaData.fertilizer} dbRef={fertilizerRef} />
                    </div>
                    <SubmitButton text="Delete Area" onClick={deleteArea} className={style.deleteContainer} />
                </div>

                {areaData.activePlan > -1 && areaData.isOpen && areaData.openTime &&
                    <div className={style.status}>
                        <CircularProgressBar plan={areaData.plans["id" + areaData.activePlan]} area={areaData} />
                    </div>}

                <div className={style.manualButtonContainer}>
                    <ManualButton isOpen={areaData.isOpen} close={areaData.close} manualOpen={areaData.manualOpen} dbPathArea={dbPathArea} />
                    {(areaData.isOpen || areaData.manualOpen) && areaData.openTime &&
                        <Timer openTime={areaData.openTime} isOpen={areaData.isOpen} />}
                </div>

                <Test areaData={areaData} dbPathArea={dbPathArea} />
            </div>
            {areaData.plans &&
                <div className={style.containerPlans}>
                    {Object.entries(areaData.plans).sort(([id1, plan1], [id2, plan2]) => {
                        return plan1.startTime.localeCompare(plan2.startTime);
                    }).map(([id, plan]) => (
                        <Plan key={id} planId={id} plan={plan} updatePlansSorted={updatePlansSorted}
                            isActive={areaData.activePlan === parseInt(id.substring(2))} />
                    ))}
                </div>}
            {addPlanPopUp && <EditPlan updatePlansSorted={updatePlansSorted} setAddPlanPopup={setAddPlanPopup} />}
        </div>
    );
}

export default Area;