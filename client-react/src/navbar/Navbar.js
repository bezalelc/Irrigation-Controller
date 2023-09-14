import { useLocation, useNavigate } from 'react-router-dom'
import { useFirebase } from '../FirebaseContext'
import style from './Navbar.module.scss'

const Navbar = () => {
    const { firebaseAuth, userId, setUserId } = useFirebase()
    const navigate = useNavigate()
    const location = useLocation();

    const buttonLogoutHandler = () => {
        firebaseAuth.signOut()
            .then(() => {
                setUserId(null)
            })
            .catch((error) => {
                console.error('Error logging out:', error);
            });
    }

    return (
        <div className={style.container}>
            <div className={style.title}>Irrigation Controller</div>
            <div className={style.buttonsContainer}>
                {userId && location.pathname !== '/home' && <button onClick={() => navigate('/home')} className={style.button}>Home</button>}
                {userId && location.pathname !== '/setting' && <button onClick={() => navigate('/setting')} className={style.button}>Setting</button>}
                {userId && location.pathname !== '/config' && <button onClick={() => navigate('/config')} className={style.button}>Config</button>}
                {userId && <button onClick={buttonLogoutHandler} className={style.button}>Logout</button>}
            </div>
        </div>
    );
}

export default Navbar;