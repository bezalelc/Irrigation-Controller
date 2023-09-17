import style from './InputText.module.scss'

const InputText = ({ Icon, type, placeholder , value, setValue}) => {
    return (
        <div className={style.container}>
            <input className={style.input} type={type} placeholder={placeholder} value={value} onChange={(event) => setValue(event.target.value)} required />
            <Icon className={style.icon} />
        </div>
    );
}

export default InputText;