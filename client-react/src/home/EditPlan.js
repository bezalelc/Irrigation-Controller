import { useEffect, useState } from "react";
import ClockTimePicker from "./ClockTimePicker";
import DurationInput from "./DurationInput";
import SelectRepeatMode from "./SelectRepeatMode";
import style from './EditPlan.module.scss'

const AddPlan = ({ plan, updatePlansSorted, setAddPlanPopup }) => {

    const [newPlan, setNewPlan] = useState(plan || {
        startTime: "08:00",
        duration: 15,
        repeatMethod: "daily",
        repeat: 0,
        lastTime: "23.4.2023"
    })

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

    // Function to center the popup
    const centerPopup = () => {
        const popup = document.querySelector(`.${style.form}`);
        if (popup) {
            const parentWidth = window.innerWidth;
            const parentHeight = window.innerHeight;
            const popupWidth = popup.offsetWidth;
            const popupHeight = popup.offsetHeight;

            const left = (parentWidth - popupWidth) / 2;
            const top = (parentHeight - popupHeight) / 2 - (parentHeight * 0.1);

            popup.style.left = left + "px";
            popup.style.top = top + "px";
        }
    };

    // Recenter the popup when the window is resized
    useEffect(() => {
        window.addEventListener("resize", centerPopup);
        return () => {
            window.removeEventListener("resize", centerPopup);
        };
    }, []);

    // Initial centering of the popup
    useEffect(() => {
        centerPopup();
    }, []);

    return (
        <div className={style.container}>
            <form onSubmit={event => event.preventDefault()} className={style.form}>
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