import style from './InputNumber.module.scss'



const InputNumber = ({ val , increament, decreament}) => {

    const startIncrementing = () => {
        const incrementInterval = setInterval(increament, 120);
        window.addEventListener("mouseup", () => {
            clearInterval(incrementInterval);
        });
    };

    const startDecrementing = () => {
        const decrementInterval = setInterval(decreament, 120);
        window.addEventListener("mouseup", () => {  
            clearInterval(decrementInterval);
        });
    };

    return (
        <div className={style.buttonsContainer}>
            <button className={`${style.button} ${style.dec}`} onMouseDown={startDecrementing}>-</button>
            <input type='number' min='0' max='340' value={val} className={style.input} readOnly />
            <button className={`${style.button} ${style.inc}`} onMouseDown={startIncrementing} >+</button>
        </div>
    );
}

export default InputNumber;