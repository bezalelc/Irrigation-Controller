import Select from 'react-select';
import style from './SelectCustom.module.scss'

const selectStyle = {
    container: (provided, state) => ({
        // Style for the container that wraps the entire Select component
        ...provided,
        boxShadow: 'none',
    }),

    control: (provided, state) => ({
        // Style for the control container (the outer container)
        ...provided,
        border: "2px solid grey",
        borderColor: state.isFocused ? '#1c96ee97' : 'gray',
        borderRadius: '10px',
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
    }),

    valueContainer: (provided, state) => ({
        // Style for the value container (where selected values are displayed)
        ...provided,
        padding: '5px 5px',
    }),

    menu: (provided, state) => ({
        // Style for the dropdown menu container
        ...provided,
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        // color:"red",
    }),

    option: (provided, state) => ({
        // Style for individual dropdown options
        ...provided,
        backgroundColor: state.isFocused ? 'rgba(0, 0, 0, 0.2)' : 'rgba(0, 0, 0, 0.7)',
        backdropFilter: 'blur(20px)',
        // backgroundColor: 'rgba(0, 0, 0, 0.2)',
        color: "#d7e8ed",
        border: "1px black solid",
    }),

    singleValue: (provided, state) => ({
        // Style for the single selected value
        ...provided,
        color: 'black',
        fontSize: "1.5rem",
        fontWeight: "500",
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        padding: '5px 5px',
        borderRadius: "10px",
    }),

    multiValue: (provided, state) => ({
        // Style for a selected value in a multi-select
        ...provided,
        backgroundColor: 'rgba(0, 0, 0, 0.2)',
        borderRadius: "10px",
        // backgroundColor: state.isFocused ? "red" :""
    }),

    multiValueLabel: (provided, state) => ({
        // Style for the label of a selected value in a multi-select
        ...provided,
        fontSize: "1.5rem",
        fontWeight: "500",
        color: "black",
    }),

    multiValueRemove: (provided, state) => ({
        // Style for the remove button of a selected value in a multi-select
        ...provided,
        color: 'gray',
    }),

    indicatorSeparator: (provided, state) => ({
        // Style for the separator between indicators (e.g., dropdown arrow and clear button)
        ...provided,
        backgroundColor: 'gray',
        color: "red",
    }),

    dropdownIndicator: (provided, state) => ({
        // Style for the dropdown indicator (the dropdown arrow)
        ...provided,
        // color: state.isHover ? "black" : "gray",
    }),

    placholder: (provided) => ({
        ...provided,
        // color: "black"
    }),

    clearIndicator: (provided, state) => ({
        ...provided,
        // color: state.isFocused ? "red" : "gray",
        // backgroundColor: state.isFocused ? "rgba(247, 151, 138, 0.451)" : "rgba(0, 0, 0, 0)",
        borderRadius: "10px",
        marginRight: "5px",
    }),

};

const SelectCustom = ({ options, isMulti, setValue, onChange }) => {
    return (
        <div className={style.container}>
            <Select styles={selectStyle} options={options} isMulti={isMulti}
                value={setValue()}
                onChange={selected => onChange(selected)} isSearchable />
        </div>
    );
}

export default SelectCustom;