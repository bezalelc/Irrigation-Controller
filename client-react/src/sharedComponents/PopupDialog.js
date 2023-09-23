import Modal from "react-bootstrap/Modal";
// import './PopupDialog.css'

const PopupDialog = ({ title, body, handleOpenModal, handleCloseModal }) => {
    // const [showModal, setShowModal] = useState(false);

    // const handleOpenModal = () => {
    //     setShowModal(true);
    // };

    // const handleCloseModal = () => {
    //     setShowModal(false);
    // };

    return (
        <div>
            {/* <button onClick={handleOpenModal}>Open Modal</button> */}
            {/* <Modal show={showModal} onHide={handleCloseModal}> */}
            <Modal show={true} onHide={handleCloseModal}>
                <Modal.Header closeButton>
                    <Modal.Title>{title}</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    {body}
                </Modal.Body>
                <Modal.Footer>
                    <button onClick={handleCloseModal}>Close</button>
                </Modal.Footer>
            </Modal>
        </div>
    );
};

export default PopupDialog;
