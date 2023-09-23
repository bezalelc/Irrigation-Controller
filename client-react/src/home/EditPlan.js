import { useState } from "react";
import ClockTimePicker from "./ClockTimePicker";
import DurationInput from "./DurationInput";
import SelectRepeatMode from "./SelectRepeatMode";
import style from './EditPlan.module.scss'

// const getToday = () => {
//     const today = new Date();
//     return `${today.getDate()}.${today.getMonth() + 1}.${today.getFullYear().toString().slice(-2)}`
// }

const planTemplate = {
    startTime: "08:00",
    duration: 15,
    repeatMethod: 0, // default = daily
    repeat: 1,
    lastTime: false
}

const AddPlan = ({ plan, updatePlansSorted, setAddPlanPopup }) => {


    const [newPlan, setNewPlan] = useState(plan || planTemplate)

    const submit = () => {
        if (plan) {
            for (const key in newPlan) {
                plan[key] = newPlan[key];
            }
            updatePlansSorted("update")
        } else {
            updatePlansSorted("add", newPlan)
        }
        setAddPlanPopup(false)
    }

    return (
        <div className={style.container}>
            <form onSubmit={event => event.preventDefault()} className={style.form} key='edit-plan-form'>
                <button onClick={() => setAddPlanPopup(false)} className={style.cancel}>X</button>
                <div className={style.title}>{plan ? "Edit plan" : "Add new plan"}</div>
                <div className={style.clockTimePickerContainer}>
                    <ClockTimePicker plan={newPlan} setPlan={setNewPlan} />
                </div>
                <hr className={style.separator} />
                <div className={style.durationInputContainer}>
                    <DurationInput plan={newPlan} setPlan={setNewPlan} />
                </div>
                <hr className={style.separator} />
                <div className={style.selectRepeatModeContainer}>
                    <SelectRepeatMode plan={newPlan} setPlan={setNewPlan} />
                </div>
                <button onClick={() => submit()} className={style.submit}>Submit</button>
            </form>
        </div>
    );
}

export default AddPlan;