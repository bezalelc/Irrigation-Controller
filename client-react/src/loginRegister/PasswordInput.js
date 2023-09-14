
import { FaLock } from 'react-icons/fa';
import style from './EmailPasswordInput.module.scss'


const PaswwordlInput = ({ password, setPassword }) => {
    return (
        <div className={style.container}>
            <input type="password" className={style.input} placeholder="Password" value={password} onChange={(event) => setPassword(event.target.value)} required />
            <FaLock className={style.icon} />
        </div>
    );
}

export default PaswwordlInput;